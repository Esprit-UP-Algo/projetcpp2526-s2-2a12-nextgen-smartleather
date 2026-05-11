#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlDatabase>

struct AuthAction {
    QDateTime dateTime;
    QString employeNom;
    QString employePrenom;
    QString action;
    QString statut;
    QString details;
};

class AuthManager
{
public:
    AuthManager();

    bool updateUserRole(int employeId, const QString &role);
    QString getUserRole(int employeId) const;
    bool resetPassword(int employeId, QString &newPassword);
    QList<AuthAction> getRecentActions(int limit = 20) const;
    void logAction(int employeId, const QString &action, const QString &statut, const QString &details = "");

    // Gestion blocage
    bool isBlocked(int employeId) const;
    int recordFailedAttempt(int employeId);   // retourne tentatives restantes
    void resetFailedAttempts(int employeId);

    static QStringList getAvailableRoles();

private:
    static const int MAX_ATTEMPTS = 3;

    bool ensureUsersTableExists();
    bool ensureAuthTableExists();
    QString generateRandomPassword(int length = 12);
};

#endif // AUTHMANAGER_H
