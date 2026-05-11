#include "authmanager.h"
#include <QSqlError>
#include <QDebug>
#include <QRandomGenerator>
#include <QCryptographicHash>

AuthManager::AuthManager()
{
    ensureUsersTableExists();
    ensureAuthTableExists();
}

bool AuthManager::ensureUsersTableExists()
{
    QSqlDatabase db = QSqlDatabase::database("oracle");
    if (!db.isOpen()) return false;

    QSqlQuery q(db);
    q.prepare("SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME = 'USERS'");
    if (q.exec() && q.next() && q.value(0).toInt() > 0) return true;

    QString sql =
        "CREATE TABLE USERS ("
        "  USER_ID NUMBER PRIMARY KEY,"
        "  EMP_ID NUMBER NOT NULL,"
        "  USERNAME VARCHAR2(100) UNIQUE NOT NULL,"
        "  PASSWORD_HASH VARCHAR2(255) NOT NULL,"
        "  ROLE VARCHAR2(50) DEFAULT 'Employe Standard',"
        "  FAILED_ATTEMPTS NUMBER DEFAULT 0,"
        "  IS_BLOCKED NUMBER(1) DEFAULT 0,"
        "  CREATED_DATE DATE DEFAULT SYSDATE,"
        "  LAST_LOGIN DATE"
        ")";
    if (!q.exec(sql)) {
        qDebug() << "Erreur creation USERS:" << q.lastError().text();
        return false;
    }

    q.exec("CREATE SEQUENCE SEQ_USERS START WITH 1 INCREMENT BY 1");
    return true;
}

bool AuthManager::ensureAuthTableExists()
{
    QSqlDatabase db = QSqlDatabase::database("oracle");
    if (!db.isOpen()) return false;

    QSqlQuery q(db);
    q.prepare("SELECT COUNT(*) FROM USER_TABLES WHERE TABLE_NAME = 'AUTH_HISTORY'");
    if (q.exec() && q.next() && q.value(0).toInt() > 0) return true;

    QString sql =
        "CREATE TABLE AUTH_HISTORY ("
        "  HISTORY_ID NUMBER PRIMARY KEY,"
        "  EMP_ID NUMBER NOT NULL,"
        "  ACTION VARCHAR2(100) NOT NULL,"
        "  STATUT VARCHAR2(50) NOT NULL,"
        "  DETAILS VARCHAR2(500),"
        "  ACTION_DATE DATE DEFAULT SYSDATE"
        ")";
    if (!q.exec(sql)) {
        qDebug() << "Erreur creation AUTH_HISTORY:" << q.lastError().text();
        return false;
    }

    q.exec("CREATE SEQUENCE SEQ_AUTH_HISTORY START WITH 1 INCREMENT BY 1");
    return true;
}

bool AuthManager::updateUserRole(int employeId, const QString &role)
{
    QSqlDatabase db = QSqlDatabase::database("oracle");
    if (!db.isOpen()) return false;

    if (!getAvailableRoles().contains(role)) return false;

    QSqlQuery q(db);
    q.prepare("SELECT COUNT(*) FROM USERS WHERE EMP_ID = :id");
    q.bindValue(":id", employeId);
    bool exists = q.exec() && q.next() && q.value(0).toInt() > 0;

    if (exists) {
        q.prepare("UPDATE USERS SET ROLE = :role WHERE EMP_ID = :id");
        q.bindValue(":role", role);
        q.bindValue(":id", employeId);
        return q.exec();
    }

    // Creer l'utilisateur
    QString username = QString("user%1").arg(employeId);
    q.prepare("SELECT FIRSTNAME, LASTNAME FROM EMP WHERE IDEMP = :id");
    q.bindValue(":id", employeId);
    if (q.exec() && q.next()) {
        QString fn = q.value(0).toString();
        QString ln = q.value(1).toString();
        if (!fn.isEmpty() && !ln.isEmpty())
            username = (fn.at(0).toLower() + ln).toLower();
    }

    q.prepare("INSERT INTO USERS (USER_ID, EMP_ID, USERNAME, PASSWORD_HASH, ROLE, FAILED_ATTEMPTS, IS_BLOCKED) "
              "VALUES (SEQ_USERS.NEXTVAL, :id, :username, 'temp_hash', :role, 0, 0)");
    q.bindValue(":id", employeId);
    q.bindValue(":username", username);
    q.bindValue(":role", role);
    return q.exec();
}

QString AuthManager::getUserRole(int employeId) const
{
    QSqlDatabase db = QSqlDatabase::database("oracle");
    if (!db.isOpen()) return "";

    QSqlQuery q(db);
    q.prepare("SELECT ROLE FROM USERS WHERE EMP_ID = :id");
    q.bindValue(":id", employeId);
    if (q.exec() && q.next())
        return q.value(0).toString();
    return "";
}

bool AuthManager::isBlocked(int employeId) const
{
    QSqlDatabase db = QSqlDatabase::database("oracle");
    if (!db.isOpen()) return false;

    QSqlQuery q(db);
    q.prepare("SELECT IS_BLOCKED FROM USERS WHERE EMP_ID = :id");
    q.bindValue(":id", employeId);
    if (q.exec() && q.next())
        return q.value(0).toInt() == 1;
    return false;
}

int AuthManager::recordFailedAttempt(int employeId)
{
    QSqlDatabase db = QSqlDatabase::database("oracle");
    if (!db.isOpen()) return 0;

    QSqlQuery q(db);
    q.prepare("SELECT FAILED_ATTEMPTS FROM USERS WHERE EMP_ID = :id");
    q.bindValue(":id", employeId);

    int attempts = 0;
    if (q.exec() && q.next())
        attempts = q.value(0).toInt();

    attempts++;
    bool block = (attempts >= MAX_ATTEMPTS);

    q.prepare("UPDATE USERS SET FAILED_ATTEMPTS = :a, IS_BLOCKED = :b WHERE EMP_ID = :id");
    q.bindValue(":a", attempts);
    q.bindValue(":b", block ? 1 : 0);
    q.bindValue(":id", employeId);
    q.exec();

    logAction(employeId, "Tentative connexion", "Echec",
              QString("Tentative %1/%2").arg(attempts).arg(MAX_ATTEMPTS));

    if (block) {
        logAction(employeId, "Blocage compte", "Bloque",
                  "Compte bloque apres trop de tentatives");
        return 0;
    }
    return MAX_ATTEMPTS - attempts;
}

void AuthManager::resetFailedAttempts(int employeId)
{
    QSqlDatabase db = QSqlDatabase::database("oracle");
    if (!db.isOpen()) return;

    QSqlQuery q(db);
    q.prepare("UPDATE USERS SET FAILED_ATTEMPTS = 0, IS_BLOCKED = 0, LAST_LOGIN = SYSDATE WHERE EMP_ID = :id");
    q.bindValue(":id", employeId);
    q.exec();
}

bool AuthManager::resetPassword(int employeId, QString &newPassword)
{
    QSqlDatabase db = QSqlDatabase::database("oracle");
    if (!db.isOpen()) return false;

    newPassword = generateRandomPassword();
    QString hash = QCryptographicHash::hash(
        newPassword.toUtf8(), QCryptographicHash::Sha256).toHex();

    QSqlQuery q(db);
    q.prepare("SELECT COUNT(*) FROM USERS WHERE EMP_ID = :id");
    q.bindValue(":id", employeId);
    bool exists = q.exec() && q.next() && q.value(0).toInt() > 0;

    if (exists) {
        q.prepare("UPDATE USERS SET PASSWORD_HASH = :h, FAILED_ATTEMPTS = 0, IS_BLOCKED = 0 WHERE EMP_ID = :id");
        q.bindValue(":h", hash);
        q.bindValue(":id", employeId);
        if (q.exec()) {
            logAction(employeId, "Reinitialisation mot de passe", "Succes", "");
            return true;
        }
        return false;
    }

    QString username = QString("user%1").arg(employeId);
    q.prepare("INSERT INTO USERS (USER_ID, EMP_ID, USERNAME, PASSWORD_HASH, ROLE, FAILED_ATTEMPTS, IS_BLOCKED) "
              "VALUES (SEQ_USERS.NEXTVAL, :id, :u, :h, 'Employe Standard', 0, 0)");
    q.bindValue(":id", employeId);
    q.bindValue(":u", username);
    q.bindValue(":h", hash);
    if (q.exec()) {
        logAction(employeId, "Reinitialisation mot de passe", "Succes", "Nouveau compte cree");
        return true;
    }
    return false;
}

void AuthManager::logAction(int employeId, const QString &action, const QString &statut, const QString &details)
{
    QSqlDatabase db = QSqlDatabase::database("oracle");
    if (!db.isOpen()) return;

    QSqlQuery q(db);
    q.prepare("INSERT INTO AUTH_HISTORY (HISTORY_ID, EMP_ID, ACTION, STATUT, DETAILS, ACTION_DATE) "
              "VALUES (SEQ_AUTH_HISTORY.NEXTVAL, :id, :action, :statut, :details, SYSDATE)");
    q.bindValue(":id", employeId);
    q.bindValue(":action", action);
    q.bindValue(":statut", statut);
    q.bindValue(":details", details);
    q.exec();
}

QList<AuthAction> AuthManager::getRecentActions(int limit) const
{
    QList<AuthAction> actions;
    QSqlDatabase db = QSqlDatabase::database("oracle");
    if (!db.isOpen()) return actions;

    QSqlQuery q(db);
    QString sql = QString(
        "SELECT * FROM ("
        "  SELECT h.ACTION_DATE, h.ACTION, h.STATUT, h.DETAILS, e.FIRSTNAME, e.LASTNAME "
        "  FROM AUTH_HISTORY h JOIN EMP e ON h.EMP_ID = e.IDEMP "
        "  ORDER BY h.ACTION_DATE DESC"
        ") WHERE ROWNUM <= %1").arg(limit);

    if (q.exec(sql)) {
        while (q.next()) {
            AuthAction a;
            a.dateTime = q.value("ACTION_DATE").toDateTime();
            a.employePrenom = q.value("FIRSTNAME").toString();
            a.employeNom = q.value("LASTNAME").toString();
            a.action = q.value("ACTION").toString();
            a.statut = q.value("STATUT").toString();
            a.details = q.value("DETAILS").toString();
            actions.append(a);
        }
    }
    return actions;
}

QStringList AuthManager::getAvailableRoles()
{
    return QStringList() << "Administrateur" << "Responsable RH"
                         << "Gestionnaire" << "Employe Standard" << "Lecture Seule";
}

QString AuthManager::generateRandomPassword(int length)
{
    const QString chars = "ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnpqrstuvwxyz23456789";
    QString pwd;
    for (int i = 0; i < length; ++i)
        pwd.append(chars.at(QRandomGenerator::global()->bounded(chars.length())));
    return pwd;
}
