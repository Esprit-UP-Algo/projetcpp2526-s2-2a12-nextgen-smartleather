#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Remplir les combos (exemple)
    ui->comboStatus->addItems({"actif", "inactif"});
    ui->comboPoste->addItems({"operateur", "technicien", "chef"});
    ui->comboRole->addItems({"admin", "user"});

    // Optionnel : date par défaut
    ui->dateHire->setDate(QDate::currentDate());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnChoosePhoto_clicked()
{
    QString f = QFileDialog::getOpenFileName(
        this, "Choose photo", "", "Images (*.png *.jpg *.jpeg)");

    if (f.isEmpty()) return;

    QPixmap p(f);
    ui->lblPhotoPreview->setPixmap(
        p.scaled(ui->lblPhotoPreview->size(),
                 Qt::KeepAspectRatio,
                 Qt::SmoothTransformation));
}

void MainWindow::on_btnAjouter_clicked()
{
    QMessageBox::information(this, "Ajouter", "Ajouter clicked");
}

void MainWindow::on_btnVider_clicked()
{
    ui->editId->clear();
    ui->editName->clear();
    ui->editAddress->clear();
    ui->editEmail->clear();
    ui->editPhone->clear();
    ui->spinSalary->setValue(0);
    ui->lblPhotoPreview->clear();
}

void MainWindow::on_btnQuitter_clicked()
{
    close();
}
