#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMap>
#include <QMessageBox>
#include <QColorDialog>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Chargement du logo "logoo" (essaie .png puis .jpg)
    QPixmap logo("C:/Users/ASUS/Documents/rayenproject1/logoo.png");
    if (logo.isNull()) {
        logo.load("C:/Users/ASUS/Documents/rayenproject1/logoo.jpg");
    }
    if (!logo.isNull()) {
        ui->logo_label->setPixmap(logo.scaled(200, 180, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    // Configuration du tableau
    ui->table_mat->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // --- LOGIQUE COULEUR (AJOUT) ---
    connect(ui->btn_color_add, &QPushButton::clicked, this, [=]() {
        QColor c = QColorDialog::getColor(Qt::white, this, "Choisir la couleur");
        if(c.isValid()) {
            ui->le_mat_color->setText(c.name());
            ui->btn_color_add->setStyleSheet(QString("background-color: %1; border: 1px solid #ccc; border-radius: 4px;").arg(c.name()));
        }
    });

    // --- LOGIQUE COULEUR (MODIFICATION) ---
    connect(ui->btn_color_upd, &QPushButton::clicked, this, [=]() {
        QColor c = QColorDialog::getColor(Qt::white, this, "Choisir la couleur");
        if(c.isValid()) {
            ui->le_upd_color->setText(c.name());
            ui->btn_color_upd->setStyleSheet(QString("background-color: %1; border: 1px solid #ccc; border-radius: 4px;").arg(c.name()));
        }
    });

    // --- FONCTIONNALITÉS CRUD ---

    // 1. Ajouter
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

        // Innovation : Notification Rupture immédiate
        if(ui->sb_mat_qty->value() < 10) {
            QMessageBox::warning(this, "Alerte Stock", "Attention : Le stock de " + ui->le_mat_name->text() + " est critique !");
        }

        QMessageBox::information(this, "Succès", "Matière ajoutée avec succès.");
        
        // Reset des champs
        ui->le_mat_name->clear(); 
        ui->le_mat_color->clear(); 
        ui->sb_mat_qty->setValue(0);
        ui->dsb_mat_price->setValue(0);
        ui->btn_color_add->setStyleSheet("background-color: white; border: 1px solid #ccc; border-radius: 4px;");
    });

    // 2. Recherche & Tri (Liste)
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

    // 3. Charger pour Modifier
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
                
                // Mise à jour couleur bouton
                ui->btn_color_upd->setStyleSheet(QString("background-color: %1; border: 1px solid #ccc; border-radius: 4px;").arg(ui->table_mat->item(i, 4)->text()));
                
                ui->dsb_upd_yield->setValue(ui->table_mat->item(i, 5)->text().toDouble());
                ui->dsb_upd_loss->setValue(ui->table_mat->item(i, 6)->text().toDouble());
                found = true;
                break;
            }
        }
        if(!found) QMessageBox::warning(this, "Erreur", "Matière introuvable.");
    });

    // 4. Confirmer Modification
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
                
                // Mise à jour du champ de recherche
                ui->le_upd_search_name->setText(ui->le_upd_name->text());
                
                QMessageBox::information(this, "Succès", "Matière modifiée.");
                found = true;
                break;
            }
        }
        if(!found) QMessageBox::warning(this, "Erreur", "Matière introuvable pour la modification.");
    });

    // 5. Supprimer
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

    // --- NAVIGATION ---
    connect(ui->btn_tab_add, &QPushButton::clicked, this, [=](){ ui->stackedWidget->setCurrentWidget(ui->page_add); });
    connect(ui->btn_tab_update, &QPushButton::clicked, this, [=](){ ui->stackedWidget->setCurrentWidget(ui->page_update); });
    connect(ui->btn_tab_delete, &QPushButton::clicked, this, [=](){ ui->stackedWidget->setCurrentWidget(ui->page_delete); });
    connect(ui->btn_tab_list, &QPushButton::clicked, this, [=](){ ui->stackedWidget->setCurrentWidget(ui->page_list); });

    // Connexion du bouton latéral "Matières premières"
    connect(ui->btn_nav_material, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->page_list);
    });

    // Configuration par défaut
    ui->stackedWidget->setCurrentWidget(ui->page_list);
    ui->factory_title->setText("Leather Factory Management");

    // --- STATISTIQUES ---
    connect(ui->tabWidget_stats, &QTabWidget::currentChanged, this, [=](int index) {
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

            // Innovation 1 (Rupture)
            if(lowStockCount > 0) {
                ui->lbl_rupture_val->setText(QString::number(lowStockCount) + " matière(s) en rupture !");
                ui->stat_card_rupture->setStyleSheet("background-color: #FF0000; color: white; border-radius: 8px; padding: 10px;");
            } else {
                ui->lbl_rupture_val->setText("Stock sain");
                ui->stat_card_rupture->setStyleSheet("background-color: #2E8B57; color: white; border-radius: 8px; padding: 10px;");
            }

            // Innovation 2 (Gaspillage)
            double avgLoss = (total > 0) ? (totalLoss / total) : 0;
            if(avgLoss > 15.0) {
                wasteAdvice = "ALERTE GASPILLAGE : Taux de perte élevé (" + QString::number(avgLoss, 'f', 1) + "%). \nAction : Réutiliser les chutes pour doublures ou accessoires.";
                ui->stat_card_waste->setStyleSheet("background-color: #FF8C00; color: white; border-radius: 8px; padding: 10px;");
            } else {
                 ui->stat_card_waste->setStyleSheet("background-color: #2E8B57; color: white; border-radius: 8px; padding: 10px;");
            }
            ui->lbl_waste_val->setText(wasteAdvice);

            // Graphique
            ui->pb_yield->setValue((total > 0) ? (int)(totalYield / total) : 0);
            ui->pb_loss->setValue((int)avgLoss);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
