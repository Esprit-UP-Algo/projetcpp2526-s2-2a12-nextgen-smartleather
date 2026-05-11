



#include "connection.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

// Initialisation du pointeur d'instance
Connection* Connection::p_instance = nullptr;

// Constructeur privé
Connection::Connection()
{
    // Initialisation de la base de données
    db = QSqlDatabase::addDatabase("QODBC");
}

// Méthode statique pour obtenir l'instance unique
Connection* Connection::instance()
{
    if (p_instance == nullptr) {
        p_instance = new Connection();
    }
    return p_instance;
}

// Méthode pour établir la connexion
bool Connection::createConnect()
{
    bool test = false;

    db.setDatabaseName("projet-2a");//inserer le nom de la source de données
    db.setUserName("rayen");//inserer nom de l'utilisateur
    db.setPassword("rayen123");//inserer mot de passe de cet utilisateur

    if (db.open()) {
        test = true;
        qDebug() << "Connexion à la base de données réussie";
        
        // Créer la table UTILISATEUR si elle n'existe pas
        createUtilisateurTableIfNeeded();
    } else {
        qDebug() << "Erreur de connexion:" << db.lastError().text();
    }

    return test;
}

// Créer la table UTILISATEUR si elle n'existe pas
void Connection::createUtilisateurTableIfNeeded()
{
    if (!db.isOpen()) {
        return;
    }

    // Vérifier si la table existe déjà
    QSqlQuery checkTable(db);
    if (checkTable.exec("SELECT 1 FROM UTILISATEUR WHERE ROWNUM = 1")) {
        qDebug() << "Table UTILISATEUR existe déjà";
        return;
    }

    // Créer la table UTILISATEUR
    QSqlQuery createTable(db);
    QString createSQL = R"(
        CREATE TABLE UTILISATEUR (
            EMAIL VARCHAR2(255) PRIMARY KEY,
            PASSWORD VARCHAR2(255) NOT NULL,
            CREATED_AT TIMESTAMP DEFAULT SYSDATE
        )
    )";

    if (createTable.exec(createSQL)) {
        qDebug() << "✓ Table UTILISATEUR créée avec succès";
    } else {
        qDebug() << "Erreur création table UTILISATEUR:" << createTable.lastError().text();
    }
}

// Fermer la connexion
void Connection::closeConnection()
{
    if (db.isOpen()) {
        db.close();
    }
}

// Destructeur privé
Connection::~Connection()
{
    closeConnection();
}





