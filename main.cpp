#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Initialiser la connexion à la base de données
    Connection* conn = Connection::instance();
    if (!conn->createConnect()) {
        QMessageBox::critical(nullptr, "Erreur de connexion", 
            "Impossible de se connecter à la base de données.\nVérifiez vos paramètres de connexion.");
        return -1;
    }
    
    MainWindow w;
    w.show();
    return a.exec();
}
