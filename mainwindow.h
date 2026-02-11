#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Page employé
    void on_btnChoosePhoto_clicked();
    void on_btnAjouter_clicked();
    void on_btnVider_clicked();
    void on_btnQuitter_clicked();

    // Navigation onglets
    void on_btnTabAdd_clicked();
    void on_btnTabEdit_clicked();
    void on_btnTabDelete_clicked();
    void on_btnTabList_clicked();

    // Fonctionnalités nouvelles
    void on_btnModifier_clicked();
    void on_btnSearchEdit_clicked();
    void on_btnSupprimer_clicked();
    void on_btnSearchDel_clicked();
    void on_btnExportPDF_clicked();
    void on_btnCalculPrime_clicked();
    void on_editSearchList_textChanged(const QString &arg1);
    void on_comboSort_currentIndexChanged(int index);

    // Login / Register
    void on_btnLogin_clicked();         // "Se connecter"
    void on_btnGotoRegister_clicked();  // "Créer un compte"
    void on_btnRegister_clicked();      // "S'inscrire"
    void on_btnGotoLogin_clicked();     // "Déjà un compte ? Se connecter"
    void on_btnEmployes_clicked();
    void on_btnAccueil_clicked();
private:
    Ui::MainWindow *ui;
    void refreshTable();
};
