#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    Connection c;
    bool test = c.createconnect();
    
    MainWindow w;
    if(test)
    {
        QSqlQuery query;
        bool tableCreated = query.exec(
            "CREATE TABLE IF NOT EXISTS employees ("
            "cin VARCHAR(20) PRIMARY KEY,"
            "nom_complet VARCHAR(100),"
            "adresse VARCHAR(200),"
            "email VARCHAR(100),"
            "telephone VARCHAR(20),"
            "poste VARCHAR(50),"
            "role VARCHAR(50),"
            "specialite VARCHAR(50),"
            "statut VARCHAR(20),"
            "sexe VARCHAR(10),"
            "salaire REAL,"
            "date_embauche DATE"
            ")");
            
        if (!tableCreated) {
            QMessageBox::warning(nullptr, "Base de données", "Erreur lors de la création de la table : " + query.lastError().text());
        }
    }
    else
    {
        QMessageBox::critical(nullptr, "Base de données", "Échec de connexion à la base de données !");
    }
    
    w.show();
    return a.exec();
}
