#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>

class Connection
{
private:
    static Connection* p_instance;
    QSqlDatabase db;
    
    Connection(); // Constructeur privé (Singleton)
    ~Connection(); // Destructeur privé

public:
    static Connection* instance();
    bool createConnect();
    void closeConnection();
    void createUtilisateurTableIfNeeded();
    QSqlDatabase getDatabase() { return db; }
    // Compat API for modules using database()
    QSqlDatabase database() const { return db; }
};

#endif // CONNECTION_H
