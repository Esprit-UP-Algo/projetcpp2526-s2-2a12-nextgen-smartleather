#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , idCounter(1)
{
    ui->setupUi(this);

    // connecter boutons
    connect(ui->btn_ajouter, &QPushButton::clicked,
            this, &MainWindow::ajouterFournisseur);

    connect(ui->btn_supprimer, &QPushButton::clicked,
            this, &MainWindow::supprimerFournisseur);

    connect(ui->btn_clear, &QPushButton::clicked,
            this, &MainWindow::viderChamps);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::ajouterFournisseur()
{
    // vérifier champs vides
    if(ui->lineEdit_nom->text().isEmpty() ||
        ui->lineEdit_type->text().isEmpty())
    {
        QMessageBox::warning(this, "Erreur",
                             "Veuillez remplir les champs obligatoires !");
        return;
    }

    int row = ui->tableFournisseurs->rowCount();
    ui->tableFournisseurs->insertRow(row);

    ui->tableFournisseurs->setItem(row, 0, new QTableWidgetItem(QString::number(idCounter++)));
    ui->tableFournisseurs->setItem(row, 1, new QTableWidgetItem(ui->lineEdit_nom->text()));
    ui->tableFournisseurs->setItem(row, 2, new QTableWidgetItem(ui->lineEdit_type->text()));
    ui->tableFournisseurs->setItem(row, 3, new QTableWidgetItem(ui->lineEdit_adresse->text()));
    ui->tableFournisseurs->setItem(row, 4, new QTableWidgetItem(ui->lineEdit_tel->text()));
    ui->tableFournisseurs->setItem(row, 5, new QTableWidgetItem(ui->lineEdit_email->text()));
    ui->tableFournisseurs->setItem(row, 6, new QTableWidgetItem(QString::number(ui->spinBox_delai->value())));
    ui->tableFournisseurs->setItem(row, 7, new QTableWidgetItem(QString::number(ui->doubleSpinBox_prix->value())));
    ui->tableFournisseurs->setItem(row, 8, new QTableWidgetItem(ui->comboBox_statut->currentText()));

    viderChamps();
}
void MainWindow::supprimerFournisseur()
{
    int row = ui->tableFournisseurs->currentRow();

    if(row < 0)
    {
        QMessageBox::information(this, "Suppression",
                                 "Sélectionnez un fournisseur !");
        return;
    }

    ui->tableFournisseurs->removeRow(row);
}
void MainWindow::viderChamps()
{
    ui->lineEdit_nom->clear();
    ui->lineEdit_type->clear();
    ui->lineEdit_adresse->clear();
    ui->lineEdit_tel->clear();
    ui->lineEdit_email->clear();

    ui->spinBox_delai->setValue(0);
    ui->doubleSpinBox_prix->setValue(0);

    ui->comboBox_statut->setCurrentIndex(0);
}
