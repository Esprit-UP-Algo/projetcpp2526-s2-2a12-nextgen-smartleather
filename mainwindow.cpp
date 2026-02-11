#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QDate>
#include <QDate>
#include <QPdfWriter>
#include <QPainter>
#include <QDesktopServices>
#include <QUrl>
#include "employe.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Page de départ : login
    ui->stackMain->setCurrentWidget(ui->pageLogin);

    // Remplir les combos sur la page employé
    ui->comboStatus->addItems({"actif", "inactif"});
    ui->comboPoste->addItems({"operateur", "technicien", "chef"});
    ui->comboRole->addItems({"admin", "user"});

    // Combo "Spécialité"
    ui->comboSpeciality->addItems({
        "Production",
        "Maintenance",
        "Qualité",
        "Logistique",
        "Informatique"
    });

    ui->dateHire->setDate(QDate::currentDate());
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ====== PAGE EMPLOYÉ ======

void MainWindow::on_btnChoosePhoto_clicked()
{
    QString f = QFileDialog::getOpenFileName(
        this, "Choisir une photo", "", "Images (*.png *.jpg *.jpeg)");

    if (f.isEmpty())
        return;

    QPixmap p(f);
    ui->lblPhotoPreview->setPixmap(
        p.scaled(ui->lblPhotoPreview->size(),
                 Qt::KeepAspectRatio,
                 Qt::SmoothTransformation));
}

void MainWindow::on_btnAjouter_clicked()
{
    // Récupérer les données saisies
    const QString id         = ui->editId->text().trimmed();
    const QString name       = ui->editName->text().trimmed();
    const QString address    = ui->editAddress->text().trimmed();
    const QString email      = ui->editEmail->text().trimmed();
    const QString phone      = ui->editPhone->text().trimmed();
    const QString status     = ui->comboStatus->currentText();
    const QString poste      = ui->comboPoste->currentText();
    const QString role       = ui->comboRole->currentText();
    const QString speciality = ui->comboSpeciality->currentText();
    const QString sexe       = ui->comboSexe->currentText(); // Assurez-vous d'avoir ajouté ce combo dans le UI
    const double  salary     = ui->spinSalary->value();
    const QDate  hireDate    = ui->dateHire->date();

    // Validation minimale
    if (id.isEmpty() || name.isEmpty()) {
        QMessageBox::warning(this, "Champs manquants", "Veuillez au minimum remplir l'ID et le Nom.");
        return;
    }

    // Instancier Employe
    Employe e(id, name, address, email, phone, poste, role, speciality, status, sexe, salary, hireDate);
    
    // Ajouter en base
    bool test = e.ajouter();
    
    if(test) {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès !");
        // Optionnel : vider le formulaire
        on_btnVider_clicked();
        
        // Optionnel : Passer à l'onglet liste pour voir le résultat ?
        // ui->stackEmployee->setCurrentIndex(3);
        // on_btnTabList_clicked(); // Si on veut déclencher le refresh
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout (CIN dupliqué ?).");
    }
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

    ui->comboStatus->setCurrentIndex(-1);
    ui->comboPoste->setCurrentIndex(-1);
    ui->comboRole->setCurrentIndex(-1);
    ui->comboSpeciality->setCurrentIndex(-1);

    ui->dateHire->setDate(QDate::currentDate());
}

void MainWindow::on_btnQuitter_clicked()
{
    close();
}

// ====== PAGES LOGIN / REGISTER ======

void MainWindow::on_btnLogin_clicked()
{
    // TODO: mettre une vraie vérification plus tard
    ui->stackMain->setCurrentWidget(ui->pageEmployee);
}

void MainWindow::on_btnGotoRegister_clicked()
{
    ui->stackMain->setCurrentWidget(ui->pageRegister);
}

void MainWindow::on_btnRegister_clicked()
{
    // TODO: logique d'inscription (vérifier mots de passe, etc.)
    QMessageBox::information(this, "Inscription", "Compte créé (exemple).");
    ui->stackMain->setCurrentWidget(ui->pageLogin);
}

void MainWindow::on_btnGotoLogin_clicked()
{
    ui->stackMain->setCurrentWidget(ui->pageLogin);
}
void MainWindow::on_btnEmployes_clicked()
{
    ui->stackMain->setCurrentWidget(ui->pageEmployee);
}

void MainWindow::on_btnAccueil_clicked()
{
    ui->stackMain->setCurrentWidget(ui->pageLogin);
}

// ====== GESTION NAVIGATION ONGLETS ======

void MainWindow::on_btnTabAdd_clicked()
{
    ui->stackEmployee->setCurrentIndex(0);
}

void MainWindow::on_btnTabEdit_clicked()
{
    ui->stackEmployee->setCurrentIndex(1);
}

void MainWindow::on_btnTabDelete_clicked()
{
    ui->stackEmployee->setCurrentIndex(2);
}


void MainWindow::refreshTable()
{
    Employe e;
    ui->tableEmployees->setModel(e.afficher());
    ui->tableEmployees->resizeColumnsToContents();
}

void MainWindow::on_btnTabList_clicked()
{
    ui->stackEmployee->setCurrentIndex(3);
    refreshTable();
}

// ====== LOGIQUE MÉTIER ======

void MainWindow::on_btnSearchEdit_clicked()
{
    QString cin = ui->editSearchEdit->text().trimmed();
    if(cin.isEmpty()) {
        QMessageBox::warning(this, "Recherche", "Veuillez entrer un CIN.");
        return;
    }

    Employe e;
    if(e.recuperer(cin)) {
        ui->editNameMod->setText(e.getNom());
        ui->editPhoneMod->setText(e.getTelephone());
        ui->spinSalaryMod->setValue(e.getSalaire());
        // On pourrait ajouter d'autres champs ici (adresse, email...) pour être complet
        
        // Stocker le CIN quelque part si on veut le bloquer ou le réutiliser
        ui->editSearchEdit->setReadOnly(true); // Bloquer pour ne pas changer d'employé en cours de modif
    } else {
        QMessageBox::information(this, "Recherche", "Aucun employé trouvé avec ce CIN.");
    }
}

void MainWindow::on_btnModifier_clicked()
{
    QString cin = ui->editSearchEdit->text().trimmed();
    QString nom = ui->editNameMod->text().trimmed();
    QString tel = ui->editPhoneMod->text().trimmed();
    double sal  = ui->spinSalaryMod->value();
    
    // On doit recréer l'objet complet pour le modifier.
    // Attention : ici on ne modifie que 3 champs pour l'exemple, 
    // mais la méthode modifier() attend que TOUT soit rempli dans l'objet.
    // Idéalement, il faudrait que recuperer() remplisse l'objet, et qu'on modifie juste ce qu'on veut.
    
    Employe e;
    if(!e.recuperer(cin)) {
         QMessageBox::critical(this, "Erreur", "L'employé a disparu ?");
         return;
    }
    
    // Mettre à jour les champs modifiés
    // On a besoin de setters dans Employe ou d'un constructeur complet.
    // Comme on n'a pas mis de setters, on va refaire un objet avec les nouvelles valeurs + anciennes valeurs
    
    Employe eNew(cin, nom, e.getAdresse(), e.getEmail(), tel, 
                 e.getPoste(), e.getRole(), e.getSpecialite(), e.getStatut(), e.getSexe(), 
                 sal, e.getDateEmbauche());
                 
    if(eNew.modifier()) {
        QMessageBox::information(this, "Succès", "Employé modifié avec succès.");
        ui->editSearchEdit->setReadOnly(false);
        ui->editSearchEdit->clear();
        ui->editNameMod->clear();
        ui->editPhoneMod->clear();
        ui->spinSalaryMod->setValue(0);
        refreshTable();
    } else {
         QMessageBox::critical(this, "Erreur", "Modification échouée.");
    }
}

void MainWindow::on_btnSearchDel_clicked()
{
    // TODO: Rechercher employé par CIN pour suppression
    QMessageBox::information(this, "Recherche", "Fonctionnalité de recherche (delete) à implémenter.");
}

void MainWindow::on_btnSupprimer_clicked()
{
    QString cin = ui->editSearchDel->text().trimmed();
    
    if(cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer le CIN de l'employé à supprimer.");
        return;
    }
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer l'employé " + cin + " ?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        Employe e;
        if(e.supprimer(cin)) {
            QMessageBox::information(this, "Succès", "Employé supprimé.");
            ui->editSearchDel->clear();
            // Refresh list if needed (optional)
        } else {
            QMessageBox::critical(this, "Erreur", "Suppression échouée (CIN introuvable ?).");
        }
    }
}

void MainWindow::on_btnExportPDF_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPdfWriter pdf(fileName);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setPageMargins(QMarginsF(15, 15, 15, 15));

    QPainter painter(&pdf);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(3000, 1000, "Liste des Employés");

    painter.setFont(QFont("Arial", 10));
    int y = 2000;
    
    QAbstractItemModel *model = ui->tableEmployees->model();
    if (!model) return;
    
    // Headers
    for(int col=0; col < model->columnCount(); col++) {
        painter.drawText(col * 800, y, model->headerData(col, Qt::Horizontal).toString());
    }
    y += 500;
    
    // Rows
    for(int row=0; row < model->rowCount(); row++) {
        for(int col=0; col < model->columnCount(); col++) {
            QString data = model->data(model->index(row, col)).toString();
            painter.drawText(col * 800, y, data);
        }
        y += 300;
        if (y > 13000) { // Nouvelle page si trop bas
            pdf.newPage();
            y = 1000;
        }
    }
    
    QMessageBox::information(this, "PDF", "Exportation réussie !");
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}

void MainWindow::on_btnCalculPrime_clicked()
{
    QModelIndex index = ui->tableEmployees->currentIndex();
    if(!index.isValid()) {
        QMessageBox::warning(this, "Prime", "Veuillez sélectionner un employé dans le tableau.");
        return;
    }
    
    // On suppose que le salaire est en colonne 10 (selon Employe::afficher)
    double salary = ui->tableEmployees->model()->data(ui->tableEmployees->model()->index(index.row(), 10)).toDouble();
    QString name = ui->tableEmployees->model()->data(ui->tableEmployees->model()->index(index.row(), 1)).toString();
    
    // Créer un objet employé temporaire juste pour le calcul
    Employe e; 
    // Hack: on utilise un constructeur complet ou on ajoute un setter pour le salaire
    // Pour l'instant, faisons un objet avec le salaire
    Employe eTemp("", "", "", "", "", "", "", "", "", "", salary, QDate::currentDate());
    
    double prime = eTemp.calculerPrime(22, 0, 100); // Hypothèse : Présent tout le mois, perf 100%
    
    QMessageBox::information(this, "Calcul Prime", 
                             "Employé : " + name + "\n"
                             "Salaire de base : " + QString::number(salary) + "\n"
                             "Prime (Présence Max, Perf 100%) : " + QString::number(prime) + " €");
}

void MainWindow::on_editSearchList_textChanged(const QString &arg1)
{
    Employe e;
    ui->tableEmployees->setModel(e.rechercher(arg1));
}

void MainWindow::on_comboSort_currentIndexChanged(int index)
{
    Employe e;
    QString critere = "nom_complet";
    QString ordre = "ASC";
    
    if(index == 1) { // Salaire
        critere = "salaire";
        ordre = "DESC";
    } else if(index == 2) { // Date
        critere = "date_embauche";
        ordre = "DESC";
    }
    
    ui->tableEmployees->setModel(e.trier(critere, ordre));
}
