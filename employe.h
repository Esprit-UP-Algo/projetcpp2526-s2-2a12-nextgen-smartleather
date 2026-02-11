#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>

class Employe
{
    QString cin;
    QString nom;
    QString adresse;
    QString email;
    QString telephone;
    QString poste;
    QString role;
    QString specialite;
    QString statut; // actif, inactif
    QString sexe;   // homme, femme
    double salaire;
    QDate date_embauche;

public:
    // Constructeurs
    Employe();
    Employe(QString cin, QString nom, QString adresse, QString email, QString phone, 
            QString poste, QString role, QString spec, QString statut, QString sexe, 
            double salaire, QDate date);

    // Getters
    QString getCin() { return cin; }
    QString getNom() { return nom; }
    QString getAdresse() { return adresse; }
    QString getEmail() { return email; }
    QString getTelephone() { return telephone; }
    QString getPoste() { return poste; }
    QString getRole() { return role; }
    QString getSpecialite() { return specialite; }
    QString getStatut() { return statut; }
    QString getSexe() { return sexe; }
    double getSalaire() { return salaire; }
    QDate getDateEmbauche() { return date_embauche; }

    // Fonctions de base CRUD
    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(QString cin);
    bool modifier();

    // Fonctions avancées
    QSqlQueryModel * rechercher(QString cin);
    bool recuperer(QString cin); // Récupérer un employé par CIN (exact match)
    QSqlQueryModel * trier(QString critere, QString ordre); // nom ASC, salary DESC etc.
    
    // Fonctions métier / Innovation
    double calculerPrime(int joursPresence, int joursAbsence, double performanceScore);
    QString checkContratExpiration();
    
    // Stats
    int getCount();
    double getSalaireMoyen();
};

#endif // EMPLOYE_H
