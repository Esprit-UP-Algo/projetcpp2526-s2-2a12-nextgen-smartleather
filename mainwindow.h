#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>
#include <QDate>
#include <QtSql/QSqlDatabase>

class QLabel;
class QPlainTextEdit;

// Structure pour un Fournisseur
struct Supplier {
    QString id;
    QString nom;
    QString type;
    QString adresse;
    QString telephone;
    QString email;
    int delai; // en jours
    double prix;
    QString statut; // "actif" ou "suspendu"
    QString notes; // notes supplémentaires
    QDate dateAjout;
    int nombreUtilisations = 0;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QVector<Supplier> suppliers; // Liste des fournisseurs
    Supplier currentSupplier; // fournisseur actuellement chargé
    QString currentSupplierId; // id du fournisseur actuellement chargé (vide si aucun)
    
    // Fonctions CRUD
    bool addSupplier(const Supplier& supplier);
    bool updateSupplier(const QString& id, const Supplier& supplier);
    bool deleteSupplier(const QString& id);
    Supplier* findSupplier(const QString& id);
    
    // Fonctions utilitaires
    void refreshSupplierTable();
    void refreshSupplierTable(const QVector<Supplier>& filtered);
    void displaySupplierStats();
    void exportSuppliersToPDF();
    QVector<Supplier> searchSuppliers(const QString& nom, const QString& type);
    QVector<Supplier> sortByPrice();
    QVector<Supplier> sortByDelai();
    Supplier* suggestBestSupplier();
    void displaySupplierHistory();
    // UI for suppliers statistics page
    QWidget *pageSuppStats = nullptr;
    QLabel *lblTotalStatsPage = nullptr;
    QLabel *lblActiveStatsPage = nullptr;
    QLabel *lblAvgPricePage = nullptr;
    QLabel *lblAvgDelaiPage = nullptr;
    QPlainTextEdit *txtStatsBreakdown = nullptr;
};
#endif // MAINWINDOW_H
