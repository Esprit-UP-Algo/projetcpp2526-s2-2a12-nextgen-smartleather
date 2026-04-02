#include "mainwindow.h"
#include "logindialog.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connection* conn = Connection::instance();
    if (!conn->createConnect()) {
        QMessageBox::critical(nullptr, "Erreur de connexion",
            "Impossible de se connecter à la base de données.\n"
            "Vérifiez vos paramètres de connexion.");
        return -1;
    }

    // Page de login
    LoginDialog login;
    login.show();
    a.exec(); // Attendre que le login soit fermé

    if (!login.isAccepted())
        return 0;

    MainWindow w;
    w.show();
    return a.exec();
}
