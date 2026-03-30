#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMap>
#include <QMessageBox>
#include <QColorDialog>
#include <QHeaderView>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QPageSize>
#include <QStandardItemModel>
#include <QPixmap>
// Database
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QDebug>
#include <QLabel>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- CONFIGURATION GLOBALE ---

    // Chargement du logo
    QPixmap logo("C:/Users/ASUS/Documents/rayenproject1/logoo.png");
    if (logo.isNull()) {
        logo.load("C:/Users/ASUS/Documents/rayenproject1/logoo.jpg");
    }
    if (!logo.isNull()) {
        ui->logo_label->setPixmap(logo.scaled(200, 180, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    ui->factory_title->setText("Leather Factory Management");

    // --- PAGE DE LOGIN PRINCIPALE (Au démarrage) ---
    
    // Navigation entre Login et Register (Page principale)
    connect(ui->btnGotoRegister_main, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget_main->setCurrentWidget(ui->page_register_main);
    });

    connect(ui->btnGotoLogin_main, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget_main->setCurrentWidget(ui->page_login_main);
    });

    // Connexion principale - Affiche le contenu après connexion
    connect(ui->btnLogin_main, &QPushButton::clicked, this, [=]() {
        QString username = ui->editLoginUser_main->text();
        QString password = ui->editLoginPassword_main->text();
        
        if (username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs.");
            return;
        }
        
        // Ici vous pouvez ajouter votre logique de vérification
        // Pour l'instant, on accepte n'importe quelle connexion
        ui->stackedWidget_main->setCurrentWidget(ui->page_main_content);
        ui->stackedWidget->setCurrentWidget(ui->page_orders);
        ui->stackedWidget_orders->setCurrentWidget(ui->page_list);
    });

    // Inscription
    connect(ui->btnRegister_main, &QPushButton::clicked, this, [=]() {
        QString name = ui->editRegName_main->text();
        QString email = ui->editRegEmail_main->text();
        QString password = ui->editRegPassword_main->text();
        QString passwordConfirm = ui->editRegPasswordConfirm_main->text();
        
        if (name.isEmpty() || email.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs.");
            return;
        }
        
        if (password != passwordConfirm) {
            QMessageBox::warning(this, "Erreur", "Les mots de passe ne correspondent pas.");
            return;
        }
        
        QMessageBox::information(this, "Succès", "Compte créé avec succès ! Vous pouvez maintenant vous connecter.");
        ui->stackedWidget_main->setCurrentWidget(ui->page_login_main);
    });

    // --- NAVIGATION PRINCIPALE (SIDEBAR) ---

    // 1. Commandes
    connect(ui->btn_nav_orders, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_orders);
        ui->stackedWidget_orders->setCurrentWidget(ui->page_list);
    });

    // 2. Matières Premières
    connect(ui->btn_nav_material, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_material);
        ui->stackedWidget_materials->setCurrentWidget(ui->page_mat_list);
    });

    // 3. Employés
    connect(ui->btn_nav_employees, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_employees);
        ui->stackMain_employees->setCurrentWidget(ui->pageEmployee);
        ui->stackEmployee->setCurrentWidget(ui->subPageAdd);
    });
    //4.Gestion Fournissuers
    connect(ui->gestion_fournisseurs, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_suppliers_2);
        ui->stackedWidget_suppliers->setCurrentWidget(ui->page_supp_add);
    });

    // --- Navigation Login/Register (Employés) ---
    connect(ui->btnGotoRegister, &QPushButton::clicked, this, [=]() {
        ui->stackMain_employees->setCurrentWidget(ui->pageRegister);
    });

    connect(ui->btnGotoLogin, &QPushButton::clicked, this, [=]() {
        ui->stackMain_employees->setCurrentWidget(ui->pageLogin);
    });

    // ============================================================
    // MODULE : GESTION DES EMPLOYÉS
    // ============================================================

    // --- Configuration du modèle pour le tableau ---
    QStandardItemModel *model = new QStandardItemModel(0, 9, this);
    model->setHorizontalHeaderLabels(QStringList() << "CIN/ID" << "Nom" << "Adresse" << "Email" << "Téléphone" << "Date Embauche" << "Poste" << "Salaire" << "Sexe");
    ui->tableEmployees->setModel(model);
    ui->tableEmployees->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //Navigation gestion fournisseurs
    connect(ui->btn_tab_supp_add, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget_suppliers->setCurrentWidget(ui->page_supp_add);
        ui->btn_tab_supp_add->setChecked(true);
        ui->btn_tab_supp_update->setChecked(false);
        ui->btn_tab_supp_delete->setChecked(false);
        ui->btn_tab_supp_list->setChecked(false);
    });

    connect(ui->btn_tab_supp_update, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget_suppliers->setCurrentWidget(ui->page_supp_update);
        ui->btn_tab_supp_add->setChecked(false);
        ui->btn_tab_supp_update->setChecked(true);
        ui->btn_tab_supp_delete->setChecked(false);
        ui->btn_tab_supp_list->setChecked(false);
    });

    connect(ui->btn_tab_supp_delete, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget_suppliers->setCurrentWidget(ui->page_supp_delete);
        ui->btn_tab_supp_add->setChecked(false);
        ui->btn_tab_supp_update->setChecked(false);
        ui->btn_tab_supp_delete->setChecked(true);
        ui->btn_tab_supp_list->setChecked(false);
    });

    connect(ui->btn_tab_supp_list, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget_suppliers->setCurrentWidget(ui->page_supp_list);
        ui->btn_tab_supp_add->setChecked(false);
        ui->btn_tab_supp_update->setChecked(false);
        ui->btn_tab_supp_delete->setChecked(false);
        ui->btn_tab_supp_list->setChecked(true);
    });
    // Create a dedicated statistics page for suppliers (programmatically)
    pageSuppStats = new QWidget(this);
    pageSuppStats->setObjectName("page_supp_statistics");
    QVBoxLayout *statsLayout = new QVBoxLayout(pageSuppStats);
    statsLayout->setContentsMargins(20,20,20,20);

    // Top row with numeric cards
    QHBoxLayout *cardsLayout = new QHBoxLayout();
    QFrame *cardTotal = new QFrame(pageSuppStats); cardTotal->setFrameShape(QFrame::StyledPanel);
    QVBoxLayout *cardTotalLayout = new QVBoxLayout(cardTotal);
    lblTotalStatsPage = new QLabel("0", cardTotal); lblTotalStatsPage->setStyleSheet("font-size:18px;font-weight:bold;color:#C68E65;");
    cardTotalLayout->addWidget(new QLabel("Total Fournisseurs")); cardTotalLayout->addWidget(lblTotalStatsPage);

    QFrame *cardActive = new QFrame(pageSuppStats); cardActive->setFrameShape(QFrame::StyledPanel);
    QVBoxLayout *cardActiveLayout = new QVBoxLayout(cardActive);
    lblActiveStatsPage = new QLabel("0", cardActive); lblActiveStatsPage->setStyleSheet("font-size:18px;font-weight:bold;color:#5cb85c;");
    cardActiveLayout->addWidget(new QLabel("Actifs")); cardActiveLayout->addWidget(lblActiveStatsPage);

    QFrame *cardPrice = new QFrame(pageSuppStats); cardPrice->setFrameShape(QFrame::StyledPanel);
    QVBoxLayout *cardPriceLayout = new QVBoxLayout(cardPrice);
    lblAvgPricePage = new QLabel("0.00 TND", cardPrice); lblAvgPricePage->setStyleSheet("font-size:18px;font-weight:bold;color:#0275d8;");
    cardPriceLayout->addWidget(new QLabel("Prix Moyen")); cardPriceLayout->addWidget(lblAvgPricePage);

    QFrame *cardDelay = new QFrame(pageSuppStats); cardDelay->setFrameShape(QFrame::StyledPanel);
    QVBoxLayout *cardDelayLayout = new QVBoxLayout(cardDelay);
    lblAvgDelaiPage = new QLabel("0 jours", cardDelay); lblAvgDelaiPage->setStyleSheet("font-size:18px;font-weight:bold;color:#f0ad4e;");
    cardDelayLayout->addWidget(new QLabel("Délai Moyen")); cardDelayLayout->addWidget(lblAvgDelaiPage);

    cardsLayout->addWidget(cardTotal);
    cardsLayout->addWidget(cardActive);
    cardsLayout->addWidget(cardPrice);
    cardsLayout->addWidget(cardDelay);
    statsLayout->addLayout(cardsLayout);

    // Breakdown text
    txtStatsBreakdown = new QPlainTextEdit(pageSuppStats);
    txtStatsBreakdown->setReadOnly(true);
    statsLayout->addWidget(txtStatsBreakdown);

    // Add the page to the stacked widget
    ui->stackedWidget_suppliers->addWidget(pageSuppStats);
    // --- Navigation Interne (Employés) ---
    connect(ui->btnTabAdd, &QPushButton::clicked, this, [=]() {
        ui->stackEmployee->setCurrentWidget(ui->subPageAdd);
    });

    connect(ui->btnTabEdit, &QPushButton::clicked, this, [=]() {
        ui->stackEmployee->setCurrentWidget(ui->subPageEdit);
    });

    connect(ui->btnTabDelete, &QPushButton::clicked, this, [=]() {
        ui->stackEmployee->setCurrentWidget(ui->subPageDelete);
    });

    connect(ui->btnTabList, &QPushButton::clicked, this, [=]() {
        ui->stackEmployee->setCurrentWidget(ui->subPageList);
        // Mettre à jour les statistiques
        int total = model->rowCount();
        ui->lblStatCount->setText("Total: " + QString::number(total));
        
        double totalSalary = 0;
        for (int i = 0; i < total; ++i) {
            QModelIndex salaryIndex = model->index(i, 7);
            if (salaryIndex.isValid()) {
                totalSalary += model->data(salaryIndex).toDouble();
            }
        }
        double avgSalary = (total > 0) ? (totalSalary / total) : 0;
        ui->lblStatAvg->setText("Moyenne Salaire: " + QString::number(avgSalary, 'f', 2));
    });

    // --- Fonctionnalité : Ajouter un Employé ---
    connect(ui->btnAjouter, &QPushButton::clicked, this, [=]() {
        QString id = ui->editId->text();
        QString name = ui->editName->text();
        QString address = ui->editAddress->text();
        QString email = ui->editEmail->text();
        QString phone = ui->editPhone->text();
        QString hireDate = ui->dateHire->text();
        QString poste = ui->comboPoste->currentText();
        double salary = ui->spinSalary->value();
        QString sexe = ui->comboSexe->currentText();

        if (id.isEmpty() || name.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Le CIN/ID et le Nom sont obligatoires.");
            return;
        }

        // Vérifier si l'ID existe déjà
        for (int i = 0; i < model->rowCount(); ++i) {
            if (model->index(i, 0).data().toString() == id) {
                QMessageBox::warning(this, "Erreur", "Un employé avec ce CIN/ID existe déjà.");
                return;
            }
        }

        // Ajouter au modèle
        int row = model->rowCount();
        model->insertRow(row);
        model->setData(model->index(row, 0), id);
        model->setData(model->index(row, 1), name);
        model->setData(model->index(row, 2), address);
        model->setData(model->index(row, 3), email);
        model->setData(model->index(row, 4), phone);
        model->setData(model->index(row, 5), hireDate);
        model->setData(model->index(row, 6), poste);
        model->setData(model->index(row, 7), salary);
        model->setData(model->index(row, 8), sexe);

        QMessageBox::information(this, "Succès", "Employé ajouté avec succès !");
    });

    // --- Vider les champs ---
    connect(ui->btnVider, &QPushButton::clicked, this, [=]() {
        ui->editId->clear();
        ui->editName->clear();
        ui->editAddress->clear();
        ui->editEmail->clear();
        ui->editPhone->clear();
        ui->dateHire->setDate(QDate::currentDate());
        ui->comboPoste->setCurrentIndex(0);
        ui->spinSalary->setValue(0);
        ui->comboSexe->setCurrentIndex(0);
        ui->comboStatus->setCurrentIndex(0);
        ui->comboSpeciality->setCurrentIndex(0);
        ui->comboRole->setCurrentIndex(0);
        ui->lblPhotoPreview->clear();
        ui->lblPhotoPreview->setText("Photo");
    });

    // --- Choisir Photo ---
    connect(ui->btnChoosePhoto, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this, "Choisir une photo", "", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!fileName.isEmpty()) {
            QPixmap pixmap(fileName);
            if (!pixmap.isNull()) {
                ui->lblPhotoPreview->setPixmap(pixmap.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        }
    });

    // --- Rechercher pour Modifier ---
    connect(ui->btnSearchEdit, &QPushButton::clicked, this, [=]() {
        QString cin = ui->editSearchEdit->text().trimmed();
        if (cin.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer un CIN/ID.");
            return;
        }

        bool found = false;
        for (int i = 0; i < model->rowCount(); ++i) {
            if (model->index(i, 0).data().toString() == cin) {
                ui->editNameMod->setText(model->index(i, 1).data().toString());
                ui->editPhoneMod->setText(model->index(i, 4).data().toString());
                ui->spinSalaryMod->setValue(model->index(i, 7).data().toDouble());
                found = true;
                break;
            }
        }

        if (!found) {
            QMessageBox::warning(this, "Erreur", "Employé introuvable.");
        }
    });

    // --- Modifier un Employé ---
    connect(ui->btnModifier, &QPushButton::clicked, this, [=]() {
        QString cin = ui->editSearchEdit->text().trimmed();
        if (cin.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez d'abord rechercher un employé.");
            return;
        }

        bool found = false;
        for (int i = 0; i < model->rowCount(); ++i) {
            if (model->index(i, 0).data().toString() == cin) {
                model->setData(model->index(i, 1), ui->editNameMod->text());
                model->setData(model->index(i, 4), ui->editPhoneMod->text());
                model->setData(model->index(i, 7), ui->spinSalaryMod->value());
                QMessageBox::information(this, "Succès", "Employé modifié avec succès !");
                found = true;
                break;
            }
        }

        if (!found) {
            QMessageBox::warning(this, "Erreur", "Employé introuvable.");
        }
    });

    // --- Supprimer un Employé ---
    connect(ui->btnSupprimer, &QPushButton::clicked, this, [=]() {
        QString cin = ui->editSearchDel->text().trimmed();
        if (cin.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer un CIN/ID.");
            return;
        }

        QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation", 
            "Êtes-vous sûr de vouloir supprimer cet employé ?", 
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            bool found = false;
            for (int i = 0; i < model->rowCount(); ++i) {
                if (model->index(i, 0).data().toString() == cin) {
                    model->removeRow(i);
                    QMessageBox::information(this, "Succès", "Employé supprimé avec succès !");
                    ui->editSearchDel->clear();
                    found = true;
                    break;
                }
            }

            if (!found) {
                QMessageBox::warning(this, "Erreur", "Employé introuvable.");
            }
        }
    });

    // --- Recherche dans la Liste ---
    connect(ui->editSearchList, &QLineEdit::textChanged, this, [=](const QString &text) {
        for (int i = 0; i < model->rowCount(); ++i) {
            bool match = false;
            for (int j = 0; j < model->columnCount(); ++j) {
                QString cellData = model->index(i, j).data().toString();
                if (cellData.contains(text, Qt::CaseInsensitive)) {
                    match = true;
                    break;
                }
            }
            ui->tableEmployees->setRowHidden(i, !match);
        }
    });

    // --- Tri dans la Liste ---
    connect(ui->comboSort, &QComboBox::currentIndexChanged, this, [=](int index) {
        if (index == 1) { // Trier par Nom
            model->sort(1, Qt::AscendingOrder);
        } else if (index == 2) { // Trier par Salaire
            model->sort(7, Qt::AscendingOrder);
        } else if (index == 3) { // Trier par Date
            model->sort(5, Qt::AscendingOrder);
        }
    });

    // --- Export PDF ---
    connect(ui->btnExportPDF, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer PDF", "", "Fichiers PDF (*.pdf)");
        if (!fileName.isEmpty()) {
            QPdfWriter writer(fileName);
            writer.setPageSize(QPageSize::A4);
            QPainter painter(&writer);

            painter.setFont(QFont("Arial", 20));
            painter.drawText(QRectF(50, 50, 500, 50), "Liste des Employés");

            painter.setFont(QFont("Arial", 10));
            int rowHeight = 30;
            int startY = 150;
            for (int i = 0; i < model->rowCount(); ++i) {
                for (int j = 0; j < model->columnCount(); ++j) {
                    QString data = model->index(i, j).data().toString();
                    painter.drawText(QRectF(50 + j * 100, startY + i * rowHeight, 100, rowHeight), data);
                }
            }

            painter.end();
            QMessageBox::information(this, "Succès", "PDF enregistré avec succès !");
        }
    });

    // --- Calculer Prime ---
    connect(ui->btnCalculPrime, &QPushButton::clicked, this, [=]() {
        int total = model->rowCount();
        if (total == 0) {
            QMessageBox::information(this, "Information", "Aucun employé dans la liste.");
            return;
        }

        double totalSalary = 0;
        for (int i = 0; i < total; ++i) {
            QModelIndex salaryIndex = model->index(i, 7);
            if (salaryIndex.isValid()) {
                totalSalary += model->data(salaryIndex).toDouble();
            }
        }
        double avgSalary = totalSalary / total;
        double prime = avgSalary * 0.1; // 10% de prime

        QMessageBox::information(this, "Calcul de Prime", 
            QString("Prime moyenne calculée: %1 TND\n(Sur la base de 10%% du salaire moyen)").arg(prime, 0, 'f', 2));
    });

    // 4. Assistant IA
    connect(ui->btn_nav_ai, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_ai);
    });


    // ============================================================
    // MODULE : GESTION DES COMMANDES (ORDERS)
    // ============================================================

    // --- Navigation Interne (Commandes) ---
    connect(ui->btn_tab_add, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget_orders->setCurrentWidget(ui->page_add);
    });
    connect(ui->btn_tab_update, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget_orders->setCurrentWidget(ui->page_update);
    });
    connect(ui->btn_tab_delete, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget_orders->setCurrentWidget(ui->page_delete);
    });
    connect(ui->btn_tab_list, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget_orders->setCurrentWidget(ui->page_list);
    });

    // --- Fonctionnalité : Ajouter une Commande ---
    connect(ui->btn_valider, &QPushButton::clicked, this, [=]() {
        QString id = ui->le_id->text();
        QString type = ui->cb_article_type->currentText();
        int qty = ui->sb_order_qty->value();
        QString email = ui->le_client_email->text();
        QString city = ui->cb_client_city->currentText();
        QString status = ui->cb_status->currentText();
        QString date = ui->de_date->text();
        QString dateDel = ui->de_date_delivery->text();
        double price = ui->dsb_price->value();
        QString proof = ui->le_proof->text();

        if (id.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID de commande.");
            return;
        }

        int row = ui->table_list->rowCount();
        ui->table_list->insertRow(row);
        ui->table_list->setItem(row, 0, new QTableWidgetItem(id));
        ui->table_list->setItem(row, 1, new QTableWidgetItem(type));
        ui->table_list->setItem(row, 2, new QTableWidgetItem(QString::number(qty)));
        ui->table_list->setItem(row, 3, new QTableWidgetItem(email));
        ui->table_list->setItem(row, 4, new QTableWidgetItem(city));
        ui->table_list->setItem(row, 5, new QTableWidgetItem(date));
        ui->table_list->setItem(row, 6, new QTableWidgetItem(dateDel));
        ui->table_list->setItem(row, 7, new QTableWidgetItem(QString::number(price)));
        ui->table_list->setItem(row, 8, new QTableWidgetItem(proof));
        ui->table_list->setItem(row, 9, new QTableWidgetItem(status));

        QMessageBox::information(this, "Succès", "La commande a été enregistrée !");

        // Reset simple
        ui->le_id->clear();
        ui->sb_order_qty->setValue(0);
    });

    // --- Modifier une Commande (Chargement) ---
    connect(ui->btn_search_update, &QPushButton::clicked, this, [=]() {
        QString id = ui->le_id_update_search->text().trimmed();
        if (id.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID à rechercher.");
            return;
        }

        bool found = false;
        for (int i = 0; i < ui->table_list->rowCount(); ++i) {
            if (ui->table_list->item(i, 0)->text() == id) {
                // Remplissage du formulaire de modification
                ui->le_id_update->setText(ui->table_list->item(i, 0)->text());
                ui->cb_type_update->setCurrentText(ui->table_list->item(i, 1)->text());
                ui->sb_qty_update->setValue(ui->table_list->item(i, 2)->text().toInt());
                ui->le_email_update->setText(ui->table_list->item(i, 3)->text());
                ui->cb_city_update->setCurrentText(ui->table_list->item(i, 4)->text());

                // Conversion des dates
                QDate dateCom = QDate::fromString(ui->table_list->item(i, 5)->text(), "dd/MM/yyyy");
                ui->de_date_update->setDate(dateCom);

                QDate dateLiv = QDate::fromString(ui->table_list->item(i, 6)->text(), "dd/MM/yyyy");
                ui->de_date_delivery_update->setDate(dateLiv);

                ui->dsb_price_update->setValue(ui->table_list->item(i, 7)->text().toDouble());
                ui->le_proof_update->setText(ui->table_list->item(i, 8)->text());
                ui->cb_status_update->setCurrentText(ui->table_list->item(i, 9)->text());

                found = true;
                break;
            }
        }

        if (!found) {
            QMessageBox::warning(this, "Introuvable", "Aucune commande trouvée avec cet ID.");
        }
    });

    // --- Modifier une Commande (Enregistrement) ---
    connect(ui->btn_update_confirm, &QPushButton::clicked, this, [=]() {
        QString id = ui->le_id_update->text(); // Champ en lecture seule rempli par la recherche
        if (id.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez d'abord charger une commande.");
            return;
        }

        for (int i = 0; i < ui->table_list->rowCount(); ++i) {
            if (ui->table_list->item(i, 0)->text() == id) {
                ui->table_list->item(i, 1)->setText(ui->cb_type_update->currentText());
                ui->table_list->item(i, 2)->setText(QString::number(ui->sb_qty_update->value()));
                ui->table_list->item(i, 3)->setText(ui->le_email_update->text());
                ui->table_list->item(i, 4)->setText(ui->cb_city_update->currentText());
                ui->table_list->item(i, 5)->setText(ui->de_date_update->text());
                ui->table_list->item(i, 6)->setText(ui->de_date_delivery_update->text());
                ui->table_list->item(i, 7)->setText(QString::number(ui->dsb_price_update->value()));
                ui->table_list->item(i, 8)->setText(ui->le_proof_update->text());
                ui->table_list->item(i, 9)->setText(ui->cb_status_update->currentText());

                QMessageBox::information(this, "Succès", "Commande modifiée avec succès.");
                // Optionnel : Retourner à la liste
                // ui->stackedWidget_orders->setCurrentWidget(ui->page_list);
                return;
            }
        }
    });

    // --- Supprimer une Commande ---
    connect(ui->btn_delete_action, &QPushButton::clicked, this, [=]() {
        QString id = ui->le_id_to_delete->text().trimmed(); // .trimmed() enlève les espaces accidentels
        if (id.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID.");
            return;
        }

        bool found = false;
        for (int i = 0; i < ui->table_list->rowCount(); ++i) {
            if (ui->table_list->item(i, 0)->text() == id) {
                ui->table_list->removeRow(i);
                QMessageBox::information(this, "Succès", "Commande supprimée.");
                ui->le_id_to_delete->clear();
                found = true;
                break;
            }
        }

        if (!found) {
            QMessageBox::warning(this, "Erreur", "ID introuvable.");
        }
    });

    // --- Statistiques Commandes ---
    connect(ui->tabWidget_stats, &QTabWidget::currentChanged, this, [=](int index) {
        if (index == 1) { // Onglet Stats
            int totalRows = ui->table_list->rowCount();
            ui->lbl_total_orders_val->setText(QString::number(totalRows));

            QMap<QString, int> cityCounts;
            for (int i = 0; i < totalRows; ++i) {
                if (ui->table_list->item(i, 4))
                    cityCounts[ui->table_list->item(i, 4)->text()]++;
            }

            QString topCity = "-";
            int maxCount = 0;
            for (auto it = cityCounts.begin(); it != cityCounts.end(); ++it) {
                if (it.value() > maxCount) {
                    maxCount = it.value();
                    topCity = it.key();
                }
            }
            ui->lbl_top_client_val->setText(topCity);
        }
    });

    // --- Export PDF Commandes ---
    connect(ui->btn_pdf, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer PDF", "", "Fichiers PDF (*.pdf)");
        if (!fileName.isEmpty()) {
            QPdfWriter writer(fileName);
            writer.setPageSize(QPageSize::A4);
            QPainter painter(&writer);

            // Titre
            painter.setFont(QFont("Arial", 20));
            painter.drawText(QRectF(50, 50, 500, 50), "Liste des Commandes");

            // Contenu du tableau
            painter.setFont(QFont("Arial", 10));
            int rowHeight = 30;
            int startY = 150;
            for (int i = 0; i < ui->table_list->rowCount(); ++i) {
                for (int j = 0; j < ui->table_list->columnCount(); ++j) {
                    if (ui->table_list->item(i, j)) {
                        painter.drawText(QRectF(50 + j * 100, startY + i * rowHeight, 100, rowHeight), ui->table_list->item(i, j)->text());
                    }
                }
            }

            painter.end();
            QMessageBox::information(this, "Succès", "PDF enregistré avec succès !");
        }
    });


    // ============================================================
    // MODULE : GESTION DES MATIÈRES PREMIÈRES (MATERIALS)
    // ============================================================

    // --- Configuration Table ---
    ui->table_mat->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // --- Navigation Interne (Matières) ---
    connect(ui->btn_tab_mat_add, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget_materials->setCurrentWidget(ui->page_mat_add);
    });
    connect(ui->btn_tab_mat_update, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget_materials->setCurrentWidget(ui->page_mat_update);
    });
    connect(ui->btn_tab_mat_delete, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget_materials->setCurrentWidget(ui->page_mat_delete);
    });
    connect(ui->btn_tab_mat_list, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget_materials->setCurrentWidget(ui->page_mat_list);
    });

    // --- Logique Couleur (Ajout) ---
    connect(ui->btn_color_add, &QPushButton::clicked, this, [=]() {
        QColor c = QColorDialog::getColor(Qt::white, this, "Choisir la couleur");
        if(c.isValid()) {
            ui->le_mat_color->setText(c.name());
            ui->btn_color_add->setStyleSheet(QString("background-color: %1; border: 1px solid #ccc; border-radius: 4px;").arg(c.name()));
        }
    });

    // --- Logique Couleur (Modification) ---
    connect(ui->btn_color_upd, &QPushButton::clicked, this, [=]() {
        QColor c = QColorDialog::getColor(Qt::white, this, "Choisir la couleur");
        if(c.isValid()) {
            ui->le_upd_color->setText(c.name());
            ui->btn_color_upd->setStyleSheet(QString("background-color: %1; border: 1px solid #ccc; border-radius: 4px;").arg(c.name()));
        }
    });

    // --- Fonctionnalité : Ajouter Matière ---
    connect(ui->btn_mat_add, &QPushButton::clicked, this, [=]() {
        if(ui->le_mat_name->text().isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Le nom est obligatoire.");
            return;
        }

        int row = ui->table_mat->rowCount();
        ui->table_mat->insertRow(row);
        ui->table_mat->setItem(row, 0, new QTableWidgetItem(ui->le_mat_name->text()));
        ui->table_mat->setItem(row, 1, new QTableWidgetItem(ui->cb_mat_type->currentText()));
        ui->table_mat->setItem(row, 2, new QTableWidgetItem(QString::number(ui->dsb_mat_price->value())));
        ui->table_mat->setItem(row, 3, new QTableWidgetItem(QString::number(ui->sb_mat_qty->value())));
        ui->table_mat->setItem(row, 4, new QTableWidgetItem(ui->le_mat_color->text()));
        ui->table_mat->setItem(row, 5, new QTableWidgetItem(QString::number(ui->dsb_mat_yield->value())));
        ui->table_mat->setItem(row, 6, new QTableWidgetItem(QString::number(ui->dsb_mat_loss->value())));

        if(ui->sb_mat_qty->value() < 10) {
            QMessageBox::warning(this, "Alerte Stock", "Attention : Le stock de " + ui->le_mat_name->text() + " est critique !");
        }

        QMessageBox::information(this, "Succès", "Matière ajoutée avec succès.");

        ui->le_mat_name->clear();
        ui->le_mat_color->clear();
        ui->sb_mat_qty->setValue(0);
        ui->dsb_mat_price->setValue(0);
        ui->btn_color_add->setStyleSheet("background-color: white; border: 1px solid #ccc; border-radius: 4px;");
    });

    // --- Recherche & Tri (Matières) ---
    connect(ui->le_search_mat, &QLineEdit::textChanged, this, [=](const QString &text) {
        for(int i=0; i<ui->table_mat->rowCount(); ++i) {
            bool match = false;
            if(ui->table_mat->item(i, 0)->text().contains(text, Qt::CaseInsensitive) ||
                ui->table_mat->item(i, 1)->text().contains(text, Qt::CaseInsensitive)) {
                match = true;
            }
            ui->table_mat->setRowHidden(i, !match);
        }
    });

    connect(ui->cb_sort_mat, &QComboBox::currentIndexChanged, this, [=](int index) {
        if(index == 1) ui->table_mat->sortItems(0, Qt::AscendingOrder); // Nom
        else if(index == 2) ui->table_mat->sortItems(2, Qt::AscendingOrder); // Prix Croissant
        else if(index == 3) ui->table_mat->sortItems(2, Qt::DescendingOrder); // Prix Décroissant
    });

    // --- Export PDF Matières ---
    connect(ui->btn_export_mat_pdf, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer PDF", "", "Fichiers PDF (*.pdf)");
        if (!fileName.isEmpty()) {
            QPdfWriter writer(fileName);
            writer.setPageSize(QPageSize::A4);
            QPainter painter(&writer);

            // Titre
            painter.setFont(QFont("Arial", 20));
            painter.drawText(QRectF(50, 50, 500, 50), "Liste des Matières Premières");

            // Contenu du tableau
            painter.setFont(QFont("Arial", 10));
            int rowHeight = 30;
            int startY = 150;
            for (int i = 0; i < ui->table_mat->rowCount(); ++i) {
                for (int j = 0; j < ui->table_mat->columnCount(); ++j) {
                    if (ui->table_mat->item(i, j)) {
                        painter.drawText(QRectF(50 + j * 100, startY + i * rowHeight, 100, rowHeight), ui->table_mat->item(i, j)->text());
                    }
                }
            }

            painter.end();
            QMessageBox::information(this, "Succès", "PDF enregistré avec succès !");
        }
    });

    // --- Modifier Matière (Chargement) ---
    connect(ui->btn_upd_load, &QPushButton::clicked, this, [=]() {
        QString name = ui->le_upd_search_name->text();
        bool found = false;
        for(int i=0; i<ui->table_mat->rowCount(); ++i) {
            if(ui->table_mat->item(i, 0)->text().compare(name, Qt::CaseInsensitive) == 0) {
                ui->le_upd_name->setText(ui->table_mat->item(i, 0)->text());
                ui->cb_upd_type->setCurrentText(ui->table_mat->item(i, 1)->text());
                ui->dsb_upd_price->setValue(ui->table_mat->item(i, 2)->text().toDouble());
                ui->sb_upd_qty->setValue(ui->table_mat->item(i, 3)->text().toInt());
                ui->le_upd_color->setText(ui->table_mat->item(i, 4)->text());

                ui->btn_color_upd->setStyleSheet(QString("background-color: %1; border: 1px solid #ccc; border-radius: 4px;").arg(ui->table_mat->item(i, 4)->text()));

                ui->dsb_upd_yield->setValue(ui->table_mat->item(i, 5)->text().toDouble());
                ui->dsb_upd_loss->setValue(ui->table_mat->item(i, 6)->text().toDouble());
                found = true;
                break;
            }
        }
        if(!found) QMessageBox::warning(this, "Erreur", "Matière introuvable.");
    });

    // --- Modifier Matière (Confirmation) ---
    connect(ui->btn_upd_confirm, &QPushButton::clicked, this, [=]() {
        QString searchName = ui->le_upd_search_name->text();
        bool found = false;
        for(int i=0; i<ui->table_mat->rowCount(); ++i) {
            if(ui->table_mat->item(i, 0)->text().compare(searchName, Qt::CaseInsensitive) == 0) {
                ui->table_mat->item(i, 0)->setText(ui->le_upd_name->text());
                ui->table_mat->item(i, 1)->setText(ui->cb_upd_type->currentText());
                ui->table_mat->item(i, 2)->setText(QString::number(ui->dsb_upd_price->value()));
                ui->table_mat->item(i, 3)->setText(QString::number(ui->sb_upd_qty->value()));
                ui->table_mat->item(i, 4)->setText(ui->le_upd_color->text());
                ui->table_mat->item(i, 5)->setText(QString::number(ui->dsb_upd_yield->value()));
                ui->table_mat->item(i, 6)->setText(QString::number(ui->dsb_upd_loss->value()));

                ui->le_upd_search_name->setText(ui->le_upd_name->text());

                QMessageBox::information(this, "Succès", "Matière modifiée.");
                found = true;
                break;
            }
        }
        if(!found) QMessageBox::warning(this, "Erreur", "Matière introuvable pour la modification.");
    });

    // --- Supprimer Matière ---
    connect(ui->btn_del_confirm, &QPushButton::clicked, this, [=]() {
        QString name = ui->le_del_name->text();
        for(int i=0; i<ui->table_mat->rowCount(); ++i) {
            if(ui->table_mat->item(i, 0)->text() == name) {
                ui->table_mat->removeRow(i);
                QMessageBox::information(this, "Succès", "Matière supprimée.");
                ui->le_del_name->clear();
                return;
            }
        }
        QMessageBox::warning(this, "Erreur", "Matière introuvable.");
    });

    // --- Statistiques Matières ---
    connect(ui->tabWidget_mat_stats, &QTabWidget::currentChanged, this, [=](int index) {
        if (index == 1) { // Onglet Stats
            int total = ui->table_mat->rowCount();
            ui->lbl_total_mat_val->setText(QString::number(total));

            int lowStockCount = 0;
            double totalYield = 0;
            double totalLoss = 0;
            QString wasteAdvice = "Niveau de perte acceptable.";

            for(int i=0; i<total; ++i) {
                if(ui->table_mat->item(i, 3)->text().toInt() < 10) lowStockCount++;
                totalYield += ui->table_mat->item(i, 5)->text().toDouble();
                totalLoss += ui->table_mat->item(i, 6)->text().toDouble();
            }

            if(lowStockCount > 0) {
                ui->lbl_rupture_val->setText(QString::number(lowStockCount) + " matière(s) en rupture !");
                ui->stat_card_rupture->setStyleSheet("background-color: #FF0000; color: white; border-radius: 8px; padding: 10px;");
            } else {
                ui->lbl_rupture_val->setText("Stock sain");
                ui->stat_card_rupture->setStyleSheet("background-color: #2E8B57; color: white; border-radius: 8px; padding: 10px;");
            }

            double avgLoss = (total > 0) ? (totalLoss / total) : 0;
            if(avgLoss > 15.0) {
                wasteAdvice = "ALERTE GASPILLAGE : Taux de perte élevé (" + QString::number(avgLoss, 'f', 1) + "%). \nAction : Réutiliser les chutes pour doublures ou accessoires.";
                ui->stat_card_waste->setStyleSheet("background-color: #FF8C00; color: white; border-radius: 8px; padding: 10px;");
            } else {
                ui->stat_card_waste->setStyleSheet("background-color: #2E8B57; color: white; border-radius: 8px; padding: 10px;");
            }
            ui->lbl_waste_val->setText(wasteAdvice);

            ui->pb_yield->setValue((total > 0) ? (int)(totalYield / total) : 0);
            ui->pb_loss->setValue((int)avgLoss);
        }
    });
    // --- Vue par défaut - Page de login au démarrage ---
    ui->stackedWidget_main->setCurrentWidget(ui->page_suppliers_2);

    // --- Database initialization for suppliers module ---
    if (!Connection::instance()->createConnect()) {
        QMessageBox::warning(this, "Erreur BD", "Impossible de se connecter à la base de données.");
    } else {
        // Load and display suppliers
        refreshSupplierTable();
    }

    // --- Suppliers: Add (ENREGISTRER) ---
    connect(ui->btnEnregistrer, &QPushButton::clicked, this, [=]() {
        Supplier s;
        s.nom = ui->txtNom->text().trimmed();
        s.type = ui->txtType->text().trimmed();
        s.adresse = ui->txtAdresse->text().trimmed();
        s.telephone = ui->txtTelephone->text().trimmed();
        s.email = ui->txtEmail->text().trimmed();
        s.delai = ui->spinDelai->value();
        s.prix = ui->spinPrix->value();
        s.statut = ui->comboStatut->currentText();
        s.notes = ui->txtNotes->toPlainText().trimmed();

        // Validation: telephone must have at least 8 digits
        int digitCount = 0;
        for (QChar ch : s.telephone) if (ch.isDigit()) ++digitCount;
        if (digitCount < 8) {
            QMessageBox::warning(this, "Erreur", "Le numéro de téléphone doit contenir au moins 8 chiffres.");
            return;
        }

        // Validation: email must contain '@'
        if (!s.email.contains('@')) {
            QMessageBox::warning(this, "Erreur", "L'adresse email doit contenir '@'.");
            return;
        }

        if (s.nom.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Le nom du fournisseur est obligatoire.");
            return;
        }

        // Check DB connection
        QSqlDatabase db = QSqlDatabase::database();
        if (!db.isOpen()) {
            QMessageBox::warning(this, "Erreur", "Connexion à la base de données perdue. Reconnexion en cours...");
            if (!Connection::instance()->createConnect()) {
                QMessageBox::warning(this, "Erreur", "Impossible de reconnecter à la base de données.");
                return;
            }
        }

        if (addSupplier(s)) {
            QMessageBox::information(this, "Succès", "Fournisseur enregistré avec succès.");
            // clear form
            ui->txtNom->clear(); ui->txtType->clear(); ui->txtAdresse->clear(); ui->txtTelephone->clear(); ui->txtEmail->clear(); ui->spinDelai->setValue(1); ui->spinPrix->setValue(0);
            refreshSupplierTable();
        } else {
            QMessageBox::warning(this, "Erreur", "Échec lors de l'enregistrement du fournisseur.");
        }
    });

    // --- Suppliers: Reset Add Form ---
    connect(ui->btnReinitialiser, &QPushButton::clicked, this, [=]() {
        ui->txtNom->clear(); ui->txtType->clear(); ui->txtAdresse->clear(); ui->txtTelephone->clear(); ui->txtEmail->clear(); ui->spinDelai->setValue(1); ui->spinPrix->setValue(0); ui->txtNotes->clear();
    });

    // --- Suppliers: Show list when clicking Liste/Stats ---
    connect(ui->btn_tab_supp_list, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget_suppliers->setCurrentWidget(ui->page_supp_list);
        // update UI selection states
        ui->btn_tab_supp_add->setChecked(false);
        ui->btn_tab_supp_update->setChecked(false);
        ui->btn_tab_supp_delete->setChecked(false);
        ui->btn_tab_supp_list->setChecked(true);
        // refresh table from DB
        refreshSupplierTable();
    });

    // --- Suppliers: Load for Update (CHARGER) ---
    connect(ui->btnChercherFournisseur, &QPushButton::clicked, this, [=]() {
        QString term = ui->txtRechercheModifier->text().trimmed();
        if (term.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez fournir un nom ou un ID.");
            return;
        }

        QSqlQuery q;
        q.prepare("SELECT ID_FOURNISSEUR, NOM, TYPE_MATIERE_FOURNIE, ADRESSE, TELEPHONE, EMAIL, DELAI_LIVRAISON_MOYEN, PRIX_MOYEN_MATIERES, STATUT FROM SUPPLIERS_MATERIALS WHERE ID_FOURNISSEUR = :term OR NOM LIKE :likeTerm");
        q.bindValue(":term", term);
        q.bindValue(":likeTerm", QString("%") + term + QString("%"));
        if (!q.exec()) {
            QMessageBox::warning(this, "Erreur DB", q.lastError().text());
            return;
        }

        if (q.next()) {
            ui->txtNomMod->setText(q.value("NOM").toString());
            ui->txtTypeMod->setText(q.value("TYPE_MATIERE_FOURNIE").toString());
            ui->txtAdresseMod->setText(q.value("ADRESSE").toString());
            ui->txtTelephoneMod->setText(q.value("TELEPHONE").toString());
            ui->txtEmailMod->setText(q.value("EMAIL").toString());
            ui->spinDelaiMod->setValue(q.value("DELAI_LIVRAISON_MOYEN").toInt());
            ui->spinPrixMod->setValue(q.value("PRIX_MOYEN_MATIERES").toDouble());
            ui->comboStatutMod->setCurrentText(q.value("STATUT").toString());
            currentSupplierId = q.value("ID_FOURNISSEUR").toString();
        } else {
            QMessageBox::information(this, "Introuvable", "Aucun fournisseur trouvé.");
        }
    });

    // --- Suppliers: Update (METTRE A JOUR) ---
    connect(ui->btnMettreAJour, &QPushButton::clicked, this, [=]() {
        if (currentSupplierId.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez d'abord charger un fournisseur à modifier.");
            return;
        }
        Supplier s;
        s.nom = ui->txtNomMod->text().trimmed();
        s.type = ui->txtTypeMod->text().trimmed();
        s.adresse = ui->txtAdresseMod->text().trimmed();
        s.telephone = ui->txtTelephoneMod->text().trimmed();
        s.email = ui->txtEmailMod->text().trimmed();
        s.delai = ui->spinDelaiMod->value();
        s.prix = ui->spinPrixMod->value();
        s.statut = ui->comboStatutMod->currentText();
        s.notes = ui->txtNotesMod->toPlainText().trimmed();

        // Validation: telephone must have at least 8 digits
        int digitCountUp = 0;
        for (QChar ch : s.telephone) if (ch.isDigit()) ++digitCountUp;
        if (digitCountUp < 8) {
            QMessageBox::warning(this, "Erreur", "Le numéro de téléphone doit contenir au moins 8 chiffres.");
            return;
        }

        // Validation: email must contain '@'
        if (!s.email.contains('@')) {
            QMessageBox::warning(this, "Erreur", "L'adresse email doit contenir '@'.");
            return;
        }

        if (updateSupplier(currentSupplierId, s)) {
            QMessageBox::information(this, "Succès", "Fournisseur mis à jour.");
            currentSupplierId.clear();
            refreshSupplierTable();
        } else {
            QMessageBox::warning(this, "Erreur", "Échec de la mise à jour.");
        }
    });

    // --- Suppliers: Load for Delete (CHARGER) ---
    connect(ui->btnChercherSupprimer, &QPushButton::clicked, this, [=]() {
        QString term = ui->txtSupprimer->text().trimmed();
        if (term.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez fournir un nom ou un ID.");
            return;
        }
        QSqlQuery q;
        q.prepare("SELECT ID_FOURNISSEUR, NOM, TYPE_MATIERE_FOURNIE, ADRESSE, TELEPHONE, EMAIL, DELAI_LIVRAISON_MOYEN, PRIX_MOYEN_MATIERES, STATUT FROM SUPPLIERS_MATERIALS WHERE ID_FOURNISSEUR = :term OR NOM LIKE :likeTerm");
        q.bindValue(":term", term);
        q.bindValue(":likeTerm", QString("%") + term + QString("%"));
        if (!q.exec()) {
            QMessageBox::warning(this, "Erreur DB", q.lastError().text());
            return;
        }
        if (q.next()) {
            QString details = QString("%1 - %2 (%3)").arg(q.value("ID_FOURNISSEUR").toString(), q.value("NOM").toString(), q.value("TYPE_MATIERE_FOURNIE").toString());
            ui->lblDetailSupp->setText(details);
            currentSupplierId = q.value("ID_FOURNISSEUR").toString();
        } else {
            ui->lblDetailSupp->setText("(Aucun fournisseur sélectionné)");
            QMessageBox::information(this, "Introuvable", "Aucun fournisseur trouvé.");
        }
    });

    // --- Suppliers: Delete (SUPPRIMER DEFINITIVEMENT) ---
    connect(ui->btnSupprimerDef, &QPushButton::clicked, this, [=]() {
        if (currentSupplierId.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Aucun fournisseur chargé pour suppression.");
            return;
        }
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer ce fournisseur ?");
        if (reply == QMessageBox::Yes) {
            if (deleteSupplier(currentSupplierId)) {
                QMessageBox::information(this, "Succès", "Fournisseur supprimé.");
                ui->lblDetailSupp->setText("(Aucun fournisseur sélectionné)");
                currentSupplierId.clear();
                refreshSupplierTable();
            } else {
                QMessageBox::warning(this, "Erreur", "Échec lors de la suppression.");
            }
        }
    });

    // --- Suppliers: Filter / List ---
    connect(ui->btnRechercher, &QPushButton::clicked, this, [=]() {
        refreshSupplierTable();
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


// ----------------------------
// Suppliers - Database helpers
// ----------------------------

bool MainWindow::addSupplier(const Supplier &supplier)
{
    qDebug() << "=== addSupplier DEBUG ===";
    qDebug() << "nom:" << supplier.nom;
    qDebug() << "type:" << supplier.type;
    qDebug() << "adresse:" << supplier.adresse;
    qDebug() << "telephone:" << supplier.telephone;
    qDebug() << "email:" << supplier.email;
    qDebug() << "delai:" << supplier.delai;
    qDebug() << "prix:" << supplier.prix;
    qDebug() << "statut:" << supplier.statut;
    
    // Generate ID: get max ID + 1
    QSqlQuery qMaxId;
    qMaxId.prepare("SELECT MAX(ID_FOURNISSEUR) as maxId FROM SUPPLIERS_MATERIALS");
    int newId = 1;
    if (qMaxId.exec()) {
        if (qMaxId.next()) {
            int maxId = qMaxId.value("maxId").toInt();
            newId = maxId + 1;
        }
    } else {
        qDebug() << "Error getting max ID:" << qMaxId.lastError().text();
    }
    qDebug() << "Generated ID:" << newId;
    
    QSqlQuery q;
    q.prepare("INSERT INTO SUPPLIERS_MATERIALS (ID_FOURNISSEUR, NOM, TYPE_MATIERE_FOURNIE, ADRESSE, TELEPHONE, EMAIL, DELAI_LIVRAISON_MOYEN, PRIX_MOYEN_MATIERES, STATUT)"
              " VALUES (:id, :nom, :type, :adresse, :tel, :email, :delai, :prix, :statut)");
    q.bindValue(":id", newId);
    q.bindValue(":nom", supplier.nom);
    q.bindValue(":type", supplier.type);
    q.bindValue(":adresse", supplier.adresse);
    q.bindValue(":tel", supplier.telephone);
    q.bindValue(":email", supplier.email);
    q.bindValue(":delai", supplier.delai);
    q.bindValue(":prix", supplier.prix);
    q.bindValue(":statut", supplier.statut);

    if (!q.exec()) {
        qDebug() << "addSupplier error:" << q.lastError().text();
        qDebug() << "Query:" << q.lastQuery();
        return false;
    }
    qDebug() << "addSupplier SUCCESS";
    return true;
}

bool MainWindow::updateSupplier(const QString &id, const Supplier &supplier)
{
    QSqlQuery q;
    q.prepare("UPDATE SUPPLIERS_MATERIALS SET NOM = :nom, TYPE_MATIERE_FOURNIE = :type, ADRESSE = :adresse, TELEPHONE = :tel, EMAIL = :email, DELAI_LIVRAISON_MOYEN = :delai, PRIX_MOYEN_MATIERES = :prix, STATUT = :statut WHERE ID_FOURNISSEUR = :id");
    q.bindValue(":nom", supplier.nom);
    q.bindValue(":type", supplier.type);
    q.bindValue(":adresse", supplier.adresse);
    q.bindValue(":tel", supplier.telephone);
    q.bindValue(":email", supplier.email);
    q.bindValue(":delai", supplier.delai);
    q.bindValue(":prix", supplier.prix);
    q.bindValue(":statut", supplier.statut);
    q.bindValue(":id", id);

    if (!q.exec()) {
        qDebug() << "updateSupplier error:" << q.lastError().text();
        return false;
    }
    return true;
}

bool MainWindow::deleteSupplier(const QString &id)
{
    QSqlQuery q;
    q.prepare("DELETE FROM SUPPLIERS_MATERIALS WHERE ID_FOURNISSEUR = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << "deleteSupplier error:" << q.lastError().text();
        return false;
    }
    return true;
}

Supplier* MainWindow::findSupplier(const QString &id)
{
    QSqlQuery q;
    q.prepare("SELECT ID_FOURNISSEUR, NOM, TYPE_MATIERE_FOURNIE, ADRESSE, TELEPHONE, EMAIL, DELAI_LIVRAISON_MOYEN, PRIX_MOYEN_MATIERES, STATUT FROM SUPPLIERS_MATERIALS WHERE ID_FOURNISSEUR = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << "findSupplier error:" << q.lastError().text();
        return nullptr;
    }
    if (q.next()) {
        currentSupplier.id = q.value("ID_FOURNISSEUR").toString();
        currentSupplier.nom = q.value("NOM").toString();
        currentSupplier.type = q.value("TYPE_MATIERE_FOURNIE").toString();
        currentSupplier.adresse = q.value("ADRESSE").toString();
        currentSupplier.telephone = q.value("TELEPHONE").toString();
        currentSupplier.email = q.value("EMAIL").toString();
        currentSupplier.delai = q.value("DELAI_LIVRAISON_MOYEN").toInt();
        currentSupplier.prix = q.value("PRIX_MOYEN_MATIERES").toDouble();
        currentSupplier.statut = q.value("STATUT").toString();
        return &currentSupplier;
    }
    return nullptr;
}

void MainWindow::refreshSupplierTable()
{
    QString filterName = ui->txtFilterNom->text().trimmed();
    QString filterType = ui->txtFilterType->text().trimmed();
    QString filterStatut = ui->txtFilterStatut->text().trimmed();

    QString queryStr = "SELECT ID_FOURNISSEUR, NOM, TYPE_MATIERE_FOURNIE, TELEPHONE, DELAI_LIVRAISON_MOYEN, PRIX_MOYEN_MATIERES, STATUT FROM SUPPLIERS_MATERIALS WHERE 1=1";
    if (!filterName.isEmpty()) queryStr += " AND NOM LIKE :name";
    if (!filterType.isEmpty()) queryStr += " AND TYPE_MATIERE_FOURNIE LIKE :type";
    if (!filterStatut.isEmpty()) queryStr += " AND STATUT LIKE :statut";

    QSqlQuery q;
    q.prepare(queryStr);
    if (!filterName.isEmpty()) q.bindValue(":name", QString("%") + filterName + QString("%"));
    if (!filterType.isEmpty()) q.bindValue(":type", QString("%") + filterType + QString("%"));
    if (!filterStatut.isEmpty()) q.bindValue(":statut", QString("%") + filterStatut + QString("%"));

    if (!q.exec()) {
        qDebug() << "refreshSupplierTable error:" << q.lastError().text();
        return;
    }

    ui->tableFournisseurs->setRowCount(0);
    int row = 0;
    int totalSup = 0;
    int activeCount = 0;
    double totalPrice = 0;
    double totalDelai = 0;

    while (q.next()) {
        ui->tableFournisseurs->insertRow(row);
        ui->tableFournisseurs->setItem(row, 0, new QTableWidgetItem(q.value("ID_FOURNISSEUR").toString()));
        ui->tableFournisseurs->setItem(row, 1, new QTableWidgetItem(q.value("NOM").toString()));
        ui->tableFournisseurs->setItem(row, 2, new QTableWidgetItem(q.value("TYPE_MATIERE_FOURNIE").toString()));
        ui->tableFournisseurs->setItem(row, 3, new QTableWidgetItem(q.value("TELEPHONE").toString()));
        ui->tableFournisseurs->setItem(row, 4, new QTableWidgetItem(q.value("DELAI_LIVRAISON_MOYEN").toString()));
        ui->tableFournisseurs->setItem(row, 5, new QTableWidgetItem(q.value("PRIX_MOYEN_MATIERES").toString()));
        ui->tableFournisseurs->setItem(row, 6, new QTableWidgetItem(q.value("STATUT").toString()));
        ui->tableFournisseurs->setItem(row, 7, new QTableWidgetItem("")); // Actions column placeholder

        // stats
        totalSup++;
        totalPrice += q.value("PRIX_MOYEN_MATIERES").toDouble();
        totalDelai += q.value("DELAI_LIVRAISON_MOYEN").toDouble();
        if (q.value("STATUT").toString().compare("Actif", Qt::CaseInsensitive) == 0) activeCount++;

        row++;
    }

    // Update stat labels if present
    if (ui->lblStatTotal) ui->lblStatTotal->setText(QString::number(totalSup));
    if (ui->lblStatActifs) ui->lblStatActifs->setText(QString::number(activeCount));
    if (ui->lblStatPrix) ui->lblStatPrix->setText(QString("%1 TND").arg((totalSup>0)?(totalPrice/totalSup):0.0, 0, 'f', 2));
    if (ui->lblStatDelai) ui->lblStatDelai->setText(QString::number((totalSup>0)?(int)(totalDelai/totalSup):0) + " jours");
}

void MainWindow::refreshSupplierTable(const QVector<Supplier> &filtered)
{
    ui->tableFournisseurs->setRowCount(0);
    for (int i = 0; i < filtered.size(); ++i) {
        int row = ui->tableFournisseurs->rowCount();
        ui->tableFournisseurs->insertRow(row);
        ui->tableFournisseurs->setItem(row, 0, new QTableWidgetItem(filtered[i].id));
        ui->tableFournisseurs->setItem(row, 1, new QTableWidgetItem(filtered[i].nom));
        ui->tableFournisseurs->setItem(row, 2, new QTableWidgetItem(filtered[i].type));
        ui->tableFournisseurs->setItem(row, 3, new QTableWidgetItem(filtered[i].telephone));
        ui->tableFournisseurs->setItem(row, 4, new QTableWidgetItem(QString::number(filtered[i].delai)));
        ui->tableFournisseurs->setItem(row, 5, new QTableWidgetItem(QString::number(filtered[i].prix)));
        ui->tableFournisseurs->setItem(row, 6, new QTableWidgetItem(filtered[i].statut));
        ui->tableFournisseurs->setItem(row, 7, new QTableWidgetItem(""));
    }
}

// Display supplier statistics (DB-backed)
void MainWindow::displaySupplierStats()
{
    QSqlQuery q;

    // Total suppliers
    int total = 0;
    if (q.exec("SELECT COUNT(*) FROM SUPPLIERS_MATERIALS")) {
        if (q.next()) total = q.value(0).toInt();
    } else {
        qDebug() << "displaySupplierStats count error:" << q.lastError().text();
    }

    // Active suppliers
    int active = 0;
    if (q.exec("SELECT COUNT(*) FROM SUPPLIERS_MATERIALS WHERE UPPER(STATUT) = 'ACTIF'")) {
        if (q.next()) active = q.value(0).toInt();
    }

    // Average price
    double avgPrice = 0.0;
    if (q.exec("SELECT AVG(PRIX_MOYEN_MATIERES) FROM SUPPLIERS_MATERIALS")) {
        if (q.next()) avgPrice = q.value(0).toDouble();
    }

    // Average delay
    double avgDelai = 0.0;
    if (q.exec("SELECT AVG(DELAI_LIVRAISON_MOYEN) FROM SUPPLIERS_MATERIALS")) {
        if (q.next()) avgDelai = q.value(0).toDouble();
    }

    // Breakdown by type
    QMap<QString, int> byType;
    if (q.exec("SELECT TYPE_MATIERE_FOURNIE, COUNT(*) FROM SUPPLIERS_MATERIALS GROUP BY TYPE_MATIERE_FOURNIE")) {
        while (q.next()) {
            QString type = q.value(0).toString();
            int cnt = q.value(1).toInt();
            byType[type] = cnt;
        }
    }

    // Update UI stat labels if present
    if (ui->lblStatTotal) ui->lblStatTotal->setText(QString::number(total));
    if (ui->lblStatActifs) ui->lblStatActifs->setText(QString::number(active));
    if (ui->lblStatPrix) ui->lblStatPrix->setText(QString("%1 TND").arg(avgPrice, 0, 'f', 2));
    if (ui->lblStatDelai) ui->lblStatDelai->setText(QString::number((int)avgDelai) + " jours");

    // Build a readable breakdown
    QString details;
    details += QString("Total fournisseurs: %1\n").arg(total);
    details += QString("Actifs: %1\n").arg(active);
    details += QString("Prix moyen: %1 TND\n").arg(QString::number(avgPrice, 'f', 2));
    details += QString("Délai moyen: %1 jours\n\n").arg((int)avgDelai);
    details += "Répartition par type:\n";
    for (auto it = byType.begin(); it != byType.end(); ++it) {
        details += QString("- %1 : %2\n").arg(it.key()).arg(it.value());
    }

    // Populate the statistics page if present
    if (lblTotalStatsPage) lblTotalStatsPage->setText(QString::number(total));
    if (lblActiveStatsPage) lblActiveStatsPage->setText(QString::number(active));
    if (lblAvgPricePage) lblAvgPricePage->setText(QString("%1 TND").arg(avgPrice, 0, 'f', 2));
    if (lblAvgDelaiPage) lblAvgDelaiPage->setText(QString::number((int)avgDelai) + " jours");
    if (txtStatsBreakdown) txtStatsBreakdown->setPlainText(details);

    // If statistics page isn't available, fall back to message box
    if (!pageSuppStats) {
        QMessageBox::information(this, "Statistiques Fournisseurs", details);
    }
}
