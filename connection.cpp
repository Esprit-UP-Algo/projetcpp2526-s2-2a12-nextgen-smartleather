#include "connection.h"

Connection::Connection()
{}

bool Connection::createconnect()
{
    bool test=false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("gestion_employes.db");
    
    if(db.open())
        test=true;
    
    return  test;
}

void Connection::closeConnection()
{
    QSqlDatabase::database().close();
}
