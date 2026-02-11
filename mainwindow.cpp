#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMap>
#include <QMessageBox>

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

    // Connexion des boutons du menu supérieur pour changer de page
    connect(ui->btn_tab_add, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_add);
    });
    connect(ui->btn_tab_update, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_update);
    });
    connect(ui->btn_tab_delete, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_delete);
    });
    connect(ui->btn_tab_list, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_list);
    });

    // --- NAVIGATION DU MENU LATÉRAL (MÉTIERS) ---

    // 1. COMMANDES : Affiche la page des commandes et le menu du haut
    connect(ui->btn_nav_orders, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_list);
        ui->top_nav->setVisible(true); // On affiche le menu CRUD (Ajouter, Modifier...)
    });

    // 2. MATÉRIEL
    connect(ui->btn_nav_material, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_material);
        ui->top_nav->setVisible(false); // On masque le menu CRUD des commandes
    });

    // 3. EMPLOYÉS
    connect(ui->btn_nav_employees, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_employees);
        ui->top_nav->setVisible(false);
    });

    // 4. ASSISTANT IA
    connect(ui->btn_nav_ai, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(ui->page_ai);
        ui->top_nav->setVisible(false);
    });

    // --- FONCTIONNALITÉ ENREGISTRER (Ajout dans le tableau) ---
    connect(ui->btn_valider, &QPushButton::clicked, this, [=]() {
        // 1. Récupération des valeurs du formulaire
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

        // 2. Vérification basique
        if (id.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID de commande.");
            return;
        }

        // 3. Ajout d'une nouvelle ligne dans le tableau (Liste)
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
    });

    // --- GESTION DES STATISTIQUES ---
    // Déclenchement lors du changement d'onglet dans la page Liste
    connect(ui->tabWidget_stats, &QTabWidget::currentChanged, this, [=](int index) {
        // L'index 1 correspond à l'onglet "Statistiques"
        if (index == 1) {
            // 1. Calcul du Total des Commandes
            int totalRows = ui->table_list->rowCount();
            ui->lbl_total_orders_val->setText(QString::number(totalRows));

            // 2. Calcul du Top Client (Ville la plus fréquente) & Données Graphique
            QMap<QString, int> cityCounts;
            QMap<QString, int> statusCounts;

            for (int i = 0; i < totalRows; ++i) {
                // La ville est à la colonne 4
                if (ui->table_list->item(i, 4))
                    cityCounts[ui->table_list->item(i, 4)->text()]++;
                
                // Le statut est à la colonne 9
                if (ui->table_list->item(i, 9))
                    statusCounts[ui->table_list->item(i, 9)->text()]++;
            }

            // Trouver la ville max
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
}

MainWindow::~MainWindow()
{
    delete ui;
}
