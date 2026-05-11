#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Employe
{
private:
    int id;
    QString nom;
    QString prenom;
    QString telephone;
    QString email;
    QString adresse;
    QString poste;
    double salaire;
    QDate dateEmbauche;
    QString statut;
    QString photoPath;
    QString lastError;

    static QString cachedTableName;
    static QString cachedStatusColumn;

    static QString resolveTableName();
    static QString resolveStatusColumn();

public:
    Employe();
    Employe(int id, QString nom, QString prenom, QString telephone, QString email,
            QString adresse, QString poste, double salaire, QDate dateEmbauche,
            QString statut, QString photoPath = "");

    // Getters
    int getId() const { return id; }
    QString getNom() const { return nom; }
    QString getPrenom() const { return prenom; }
    QString getTelephone() const { return telephone; }
    QString getEmail() const { return email; }
    QString getAdresse() const { return adresse; }
    QString getPoste() const { return poste; }
    double getSalaire() const { return salaire; }
    QDate getDateEmbauche() const { return dateEmbauche; }
    QString getStatut() const { return statut; }
    QString getPhotoPath() const { return photoPath; }
    QString getLastError() const { return lastError; }

    // Setters
    void setId(int v) { id = v; }
    void setNom(QString v) { nom = v; }
    void setPrenom(QString v) { prenom = v; }
    void setTelephone(QString v) { telephone = v; }
    void setEmail(QString v) { email = v; }
    void setAdresse(QString v) { adresse = v; }
    void setPoste(QString v) { poste = v; }
    void setSalaire(double v) { salaire = v; }
    void setDateEmbauche(QDate v) { dateEmbauche = v; }
    void setStatut(QString v) { statut = v; }
    void setPhotoPath(QString v) { photoPath = v; }

    // CRUD
    bool ajouter();
    bool modifier();
    bool supprimer(int id);
    QSqlQueryModel* afficher();
    QSqlQueryModel* rechercher(QString critere);
    bool existe(int id);
    int getNextId();

    static QString getTableNameForQueries();
    static QString getStatusColumnForQueries();
};

#endif // EMPLOYE_H
