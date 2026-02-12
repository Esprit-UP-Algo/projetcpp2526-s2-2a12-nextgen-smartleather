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
    ui->stackedWidget_main->setCurrentWidget(ui->page_login_main);
}

MainWindow::~MainWindow()
{
    delete ui;
}
