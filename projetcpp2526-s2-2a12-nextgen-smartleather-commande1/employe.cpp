#include "employe.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <utility>

QString Employe::cachedTableName = "";
QString Employe::cachedStatusColumn = "";

Employe::Employe()
    : id(0), salaire(0.0), dateEmbauche(QDate::currentDate()), statut("Actif") {}

Employe::Employe(int id, QString nom, QString prenom, QString telephone, QString email,
                 QString adresse, QString poste, double salaire, QDate dateEmbauche,
                 QString statut, QString photoPath)
    : id(id), nom(nom), prenom(prenom), telephone(telephone), email(email),
      adresse(adresse), poste(poste), salaire(salaire), dateEmbauche(dateEmbauche),
      statut(statut), photoPath(photoPath) {}

bool Employe::ajouter()
{
    QSqlDatabase db = QSqlDatabase::database("oracle");
    if (!db.isOpen()) return false;

    const QString table = resolveTableName();
    const QString statusCol = resolveStatusColumn();

    QSqlQuery q(db);
    q.prepare("INSERT INTO " + table +
              " (LASTNAME, FIRSTNAME, CONTACT, ADRESSE, DESCRIPTION, POSTE, SALARY, HIREDATE, " + statusCol + ") "
              "VALUES (:ln, :fn, :contact, :adresse, :desc, :poste, :salary, :hiredate, :statut)");
    q.bindValue(":ln", nom);
    q.bindValue(":fn", prenom);
    q.bindValue(":contact", telephone);
    q.bindValue(":adresse", adresse);
    q.bindValue(":desc", email);
    q.bindValue(":poste", poste);
    q.bindValue(":salary", salaire);
    q.bindValue(":hiredate", dateEmbauche);
    q.bindValue(":statut", statut);

    if (q.exec()) {
        QSqlQuery maxQ(db);
        maxQ.exec("SELECT MAX(IDEMP) FROM " + table);
        if (maxQ.next()) id = maxQ.value(0).toInt();
        lastError = "";
        return true;
    }
    lastError = q.lastError().text();
    qDebug() << "Erreur ajouter:" << lastError;
    return false;
}

bool Employe::modifier()
{
    QSqlDatabase db = QSqlDatabase::database("oracle");
    if (!db.isOpen() || id <= 0) return false;

    const QString table = resolveTableName();
    const QString statusCol = resolveStatusColumn();

    QSqlQuery q(db);
    q.prepare("UPDATE " + table +
              " SET LASTNAME=:ln, FIRSTNAME=:fn, CONTACT=:contact, ADRESSE=:adresse, "
              "DESCRIPTION=:desc, POSTE=:poste, SALARY=:salary, HIREDATE=:hiredate, "
              + statusCol + "=:statut WHERE IDEMP=:id");
    q.bindValue(":ln", nom);
    q.bindValue(":fn", prenom);
    q.bindValue(":contact", telephone);
    q.bindValue(":adresse", adresse);
    q.bindValue(":desc", email);
    q.bindValue(":poste", poste);
    q.bindValue(":salary", salaire);
    q.bindValue(":hiredate", dateEmbauche);
    q.bindValue(":statut", statut);
    q.bindValue(":id", id);

    if (q.exec()) { lastError = ""; return true; }
    lastError = q.lastError().text();
    return false;
}

bool Employe::supprimer(int empId)
{
    QSqlDatabase db = QSqlDatabase::database("oracle");
    if (!db.isOpen() || empId <= 0) return false;

    QSqlQuery q(db);
    q.prepare("DELETE FROM " + resolveTableName() + " WHERE IDEMP=:id");
    q.bindValue(":id", empId);
    if (q.exec()) { lastError = ""; return true; }
    lastError = q.lastError().text();
    return false;
}

QSqlQueryModel* Employe::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlDatabase db = QSqlDatabase::database("oracle");
    if (!db.isOpen()) return model;

    const QString table = resolveTableName();
    const QString statusCol = resolveStatusColumn();

    QSqlQuery q(db);
    if (q.exec("SELECT IDEMP, LASTNAME || ' ' || FIRSTNAME AS FULL_NAME, FIRSTNAME, "
               "POSTE, CONTACT, " + statusCol + " FROM " + table + " ORDER BY LASTNAME")) {
        model->setQuery(std::move(q));
        model->setHeaderData(0, Qt::Horizontal, "ID");
        model->setHeaderData(1, Qt::Horizontal, "Nom");
        model->setHeaderData(2, Qt::Horizontal, "Prenom");
        model->setHeaderData(3, Qt::Horizontal, "Poste");
        model->setHeaderData(4, Qt::Horizontal, "Telephone");
        model->setHeaderData(5, Qt::Horizontal, "Statut");
    }
    return model;
}

QSqlQueryModel* Employe::rechercher(QString critere)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlDatabase db = QSqlDatabase::database("oracle");
    if (!db.isOpen()) return model;

    const QString table = resolveTableName();
    const QString statusCol = resolveStatusColumn();

    QSqlQuery q(db);
    q.prepare("SELECT IDEMP, LASTNAME || ' ' || FIRSTNAME AS FULL_NAME, FIRSTNAME, "
              "POSTE, CONTACT, " + statusCol + " FROM " + table +
              " WHERE TO_CHAR(IDEMP) LIKE :c OR LASTNAME LIKE :c OR FIRSTNAME LIKE :c OR POSTE LIKE :c "
              "ORDER BY LASTNAME");
    q.bindValue(":c", "%" + critere + "%");
    if (q.exec()) {
        model->setQuery(std::move(q));
        model->setHeaderData(0, Qt::Horizontal, "ID");
        model->setHeaderData(1, Qt::Horizontal, "Nom");
        model->setHeaderData(2, Qt::Horizontal, "Prenom");
        model->setHeaderData(3, Qt::Horizontal, "Poste");
        model->setHeaderData(4, Qt::Horizontal, "Telephone");
        model->setHeaderData(5, Qt::Horizontal, "Statut");
    }
    return model;
}

bool Employe::existe(int empId)
{
    QSqlDatabase db = QSqlDatabase::database("oracle");
    if (!db.isOpen()) return false;
    QSqlQuery q(db);
    q.prepare("SELECT IDEMP FROM " + resolveTableName() + " WHERE IDEMP=:id");
    q.bindValue(":id", empId);
    return q.exec() && q.next();
}

int Employe::getNextId()
{
    QSqlDatabase db = QSqlDatabase::database("oracle");
    if (!db.isOpen()) return 1;
    QSqlQuery q(db);
    if (q.exec("SELECT SEQ_EMP.NEXTVAL FROM DUAL") && q.next())
        return q.value(0).toInt();
    if (q.exec("SELECT MAX(IDEMP) FROM " + resolveTableName()) && q.next())
        return q.value(0).toInt() + 1;
    return 1;
}

QString Employe::resolveTableName()
{
    if (!cachedTableName.isEmpty()) return cachedTableName;

    QSqlDatabase db = QSqlDatabase::database("oracle");
    QSqlQuery q(db);

    if (q.exec("SELECT 1 FROM EMP WHERE 1=0")) {
        cachedTableName = "EMP";
        return cachedTableName;
    }

    for (const QString &owner : {"AFRA", "SYSTEM", "SYS"}) {
        if (q.exec("SELECT 1 FROM " + owner + ".EMP WHERE 1=0")) {
            cachedTableName = owner + ".EMP";
            return cachedTableName;
        }
    }

    cachedTableName = "EMP";
    return cachedTableName;
}

QString Employe::resolveStatusColumn()
{
    if (!cachedStatusColumn.isEmpty()) return cachedStatusColumn;

    QSqlDatabase db = QSqlDatabase::database("oracle");
    QSqlQuery q(db);
    const QString table = resolveTableName();

    if (q.exec("SELECT STATUS FROM " + table + " WHERE 1=0")) {
        cachedStatusColumn = "STATUS";
    } else {
        cachedStatusColumn = "STATUT";
    }
    return cachedStatusColumn;
}

QString Employe::getTableNameForQueries() { return resolveTableName(); }
QString Employe::getStatusColumnForQueries() { return resolveStatusColumn(); }
