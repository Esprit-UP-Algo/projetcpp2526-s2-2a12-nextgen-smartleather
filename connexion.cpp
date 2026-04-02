#include "connection.h"
#include <QSqlError>
#include <QDebug>

// Initialisation du pointeur d'instance
Connection* Connection::p_instance = nullptr;

namespace {
const char* kConnectionName = "smartleather_connection";

QSqlDatabase ensureDatabaseHandle()
{
    if (QSqlDatabase::contains(QLatin1String(kConnectionName))) {
        return QSqlDatabase::database(QLatin1String(kConnectionName));
    }
    return QSqlDatabase::addDatabase("QODBC", QLatin1String(kConnectionName));
}
}

// Constructeur prive
Connection::Connection()
    : db(ensureDatabaseHandle())
{
}

// Methode statique pour obtenir l'instance unique
Connection* Connection::instance()
{
    if (p_instance == nullptr) {
        p_instance = new Connection();
    }
    return p_instance;
}

// Methode pour etablir la connexion
bool Connection::createConnect()
{
    db = ensureDatabaseHandle();

    if (db.isOpen()) {
        return true;
    }

    db.setDatabaseName("smartleather");
    db.setUserName("smartleather");
    db.setPassword("smartleather123");

    if (db.open()) {
        qDebug() << "Connexion reussie a Oracle via QODBC";
        return true;
    }

    qDebug() << "Erreur connexion :" << db.lastError().text();
    return false;
}

// Fermer la connexion
void Connection::closeConnection()
{
    if (db.isOpen()) {
        db.close();
    }
}

// Destructeur
Connection::~Connection()
{
    closeConnection();
}
