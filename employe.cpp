#include "employe.h"
#include <QSqlQuery>
#include <QDebug>
#include <QObject>

Employe::Employe()
{
    salaire = 0;
}

Employe::Employe(QString cin, QString nom, QString adresse, QString email, QString phone,
                 QString poste, QString role, QString spec, QString statut, QString sexe,
                 double salaire, QDate date)
{
    this->cin = cin;
    this->nom = nom;
    this->adresse = adresse;
    this->email = email;
    this->telephone = phone;
    this->poste = poste;
    this->role = role;
    this->specialite = spec;
    this->statut = statut;
    this->sexe = sexe;
    this->salaire = salaire;
    this->date_embauche = date;
}

bool Employe::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO employees (cin, nom_complet, adresse, email, telephone, "
                  "poste, role, specialite, statut, sexe, salaire, date_embauche) "
                  "VALUES (:cin, :nom, :adresse, :email, :tel, :poste, :role, :spec, :stat, :sexe, :sal, :date)");
    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":adresse", adresse);
    query.bindValue(":email", email);
    query.bindValue(":tel", telephone);
    query.bindValue(":poste", poste);
    query.bindValue(":role", role);
    query.bindValue(":spec", specialite);
    query.bindValue(":stat", statut);
    query.bindValue(":sexe", sexe);
    query.bindValue(":sal", salaire);
    query.bindValue(":date", date_embauche);

    return query.exec();
}

bool Employe::supprimer(QString cin)
{
    QSqlQuery query;
    query.prepare("DELETE FROM employees WHERE cin = :cin");
    query.bindValue(":cin", cin);
    return query.exec();
}

QSqlQueryModel * Employe::afficher()
{
    QSqlQueryModel * model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM employees");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Rôle"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Spécialité"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Sexe"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(11, Qt::Horizontal, QObject::tr("Date calculée"));
    return model;
}

bool Employe::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE employees SET nom_complet= :nom, adresse= :adresse, email= :email, "
                  "telephone= :tel, poste= :poste, role= :role, specialite= :spec, statut= :stat, "
                  "sexe= :sexe, salaire= :sal, date_embauche= :date WHERE cin= :cin");
    
    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":adresse", adresse);
    query.bindValue(":email", email);
    query.bindValue(":tel", telephone);
    query.bindValue(":poste", poste);
    query.bindValue(":role", role);
    query.bindValue(":spec", specialite);
    query.bindValue(":stat", statut);
    query.bindValue(":sexe", sexe);
    query.bindValue(":sal", salaire);
    query.bindValue(":date", date_embauche);

    return query.exec();
}

bool Employe::recuperer(QString cin)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM employees WHERE cin = :cin");
    query.bindValue(":cin", cin);
    if(query.exec() && query.next()) {
        this->cin = query.value("cin").toString();
        this->nom = query.value("nom_complet").toString();
        this->adresse = query.value("adresse").toString();
        this->email = query.value("email").toString();
        this->telephone = query.value("telephone").toString();
        this->poste = query.value("poste").toString();
        this->role = query.value("role").toString();
        this->specialite = query.value("specialite").toString();
        this->statut = query.value("statut").toString();
        this->sexe = query.value("sexe").toString();
        this->salaire = query.value("salaire").toDouble();
        this->date_embauche = query.value("date_embauche").toDate();
        return true;
    }
    return false;
}

QSqlQueryModel * Employe::trier(QString critere, QString ordre)
{
    QSqlQueryModel * model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM employees ORDER BY " + critere + " " + ordre);
    return model;
}

QSqlQueryModel * Employe::rechercher(QString val)
{
    QSqlQueryModel * model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM employees WHERE cin LIKE :val OR nom_complet LIKE :val");
    query.bindValue(":val", val + "%");
    query.exec();
    
    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    // ... plus de headers si on veut
    return model;
}

double Employe::calculerPrime(int joursPresence, int joursAbsence, double performanceScore)
{
    // Formule simple pour l'exemple
    // Prime = Salaire * (Presence / Total) * Performance
    // Supposons 22 jours ouvrés par mois
    int totalJours = joursPresence + joursAbsence;
    if (totalJours == 0) return 0.0;
    
    double ratio = (double)joursPresence / totalJours;
    double prime = salaire * 0.10 * ratio * (performanceScore / 100.0);
    return prime;
}

QString Employe::checkContratExpiration()
{
    // Dummy implementation using hire date + 1 year for example
    QDate expiry = date_embauche.addYears(1);
    int daysLeft = QDate::currentDate().daysTo(expiry);
    
    if (daysLeft > 0 && daysLeft <= 30) {
        return "Attention: Contrat expire dans " + QString::number(daysLeft) + " jours.";
    }
    return "";
}

int Employe::getCount() {
    QSqlQuery query("SELECT COUNT(*) FROM employees");
    if(query.next()) return query.value(0).toInt();
    return 0;
}

double Employe::getSalaireMoyen() {
    QSqlQuery query("SELECT AVG(salaire) FROM employees");
    if(query.next()) return query.value(0).toDouble();
    return 0.0;
}
