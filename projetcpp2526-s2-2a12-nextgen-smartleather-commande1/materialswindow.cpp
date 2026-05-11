#include "materialswindow.h"
#include "ui_materialswindow.h"
#include "connection.h"
#include <QMap>
#include <QDebug>
#include <QMessageBox>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QColor>
#include <QHeaderView>
#include <QIntValidator>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QPageLayout>
#include <QDateTime>
#include <QDir>
#include <QCoreApplication>
#include <QtSql>
#include <QList>
#include <QtGlobal>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QLegend>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QSqlRecord>
#include <QLayout>
#include <QButtonGroup>
#include <QAbstractSpinBox>
#include <QScrollArea>

// Helpers forward declarations
static void cleanupChildConstraintRows(QSqlDatabase &db, int idValue);

QString gMaterialsTableName = "MATIERE_PREMIERE";
static bool gHasNomMatiere = false;
static const QMap<QString, QString> gLeatherColors = {
    {"Noir profond", "#1F1A17"},
    {"Ebene", "#2B211B"},
    {"Brun espresso", "#4A2F23"},
    {"Chocolat", "#5C3A2E"},
    {"Acajou", "#7A3E2E"},
    {"Cognac", "#9A5B2D"},
    {"Caramel", "#B77A45"},
    {"Miel", "#C8965B"},
    {"Taupe fonce", "#7C6A58"}
};


MaterialsWindow::MaterialsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MaterialsWindow)
{
    ui->setupUi(this);

    // Layout principal réactif : sidebar + contenu (top nav + pages)
    if (ui->centralwidget && ui->sidebar && ui->top_nav && ui->stackedWidget) {
        auto *mainLayout = new QHBoxLayout(ui->centralwidget);
        mainLayout->setContentsMargins(10, 10, 10, 10);
        mainLayout->setSpacing(12);

        ui->sidebar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        ui->sidebar->setStyleSheet(
            "background: qlineargradient(x1:0,y1:0,x2:0,y2:1, stop:0 #3a2a22, stop:1 #2b2019);"
            "color: #f4e9dd;"
            "border-right: 2px solid #C68E65;"
            );
        mainLayout->addWidget(ui->sidebar);

        auto *contentLayout = new QVBoxLayout();
        contentLayout->setContentsMargins(0, 0, 0, 0);
        contentLayout->setSpacing(8);

        ui->top_nav->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        contentLayout->addWidget(ui->top_nav);
        ui->stackedWidget->setContentsMargins(6, 4, 6, 10);

        // Activer le scroll vertical sur les pages (ex: Stats longue)
        auto *contentScroll = new QScrollArea(ui->centralwidget);
        contentScroll->setWidgetResizable(true);
        contentScroll->setFrameShape(QFrame::NoFrame);
        contentScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        contentScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        contentScroll->viewport()->setStyleSheet("background-color: #fdf7f2;");
        ui->top_nav->setStyleSheet("background-color: #fdf7f2;");
        ui->stackedWidget->setParent(contentScroll);
        contentScroll->setWidget(ui->stackedWidget);
        contentLayout->addWidget(contentScroll, 1);

        mainLayout->addLayout(contentLayout, 1);
        ui->centralwidget->setLayout(mainLayout);
    }

    // Supprimer le titre de gestion dans la sidebar pour un aspect plus épuré
    if (ui->factory_title) {
        ui->factory_title->hide();
    }

    // Harmoniser le fond avec la page Commandes
    const QString matBackground = "background-color: #fdf7f2;";
    auto setBg = [&](QWidget *w){ if (w) w->setStyleSheet(matBackground); };
    setBg(ui->centralwidget);
    setBg(ui->stackedWidget);
    setBg(ui->page_add);
    setBg(ui->page_update);
    setBg(ui->page_delete);
    setBg(ui->page_list);
    setBg(ui->page_stats);

    // Encadrement des pages pour matcher le style Commandes
    const QString panelStyle = "background:#ffffff;border:1px solid #C68E65;border-radius:10px;";
    auto applyPanel = [&](QWidget *page){
        if (!page) return;
        page->setStyleSheet(panelStyle);
        if (auto *lay = page->layout()) {
            lay->setContentsMargins(20, 20, 20, 20);
            lay->setSpacing(16);
        }
    };
    applyPanel(ui->page_add);
    applyPanel(ui->page_update);
    applyPanel(ui->page_delete);
    applyPanel(ui->page_list);
    applyPanel(ui->page_stats);


    // Harmoniser la hauteur et retirer les flèches des spinbox pour un aspect "barre"
    const int fieldHeight = 52;
    const QList<QAbstractSpinBox*> spinInputs = {
        ui->dsb_mat_price, ui->sb_mat_qty, ui->dsb_mat_yield, ui->dsb_mat_loss,
        ui->dsb_upd_price, ui->sb_upd_qty, ui->dsb_upd_yield, ui->dsb_upd_loss
    };
    for (auto *spin : spinInputs) {
        if (!spin) continue;
        spin->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin->setMinimumHeight(fieldHeight);
        spin->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }

    const QList<QWidget*> lineInputs = {
        ui->le_mat_name, ui->le_mat_color, ui->le_upd_search_name, ui->le_upd_name,
        ui->le_upd_color, ui->le_del_name, ui->le_search_mat
    };
    for (auto *w : lineInputs) {
        if (!w) continue;
        w->setMinimumHeight(fieldHeight);
        w->setMaximumHeight(fieldHeight);
        w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }

    // Ajuster la barre de recherche (Modifier) pour laisser la place au bouton
    if (ui->le_upd_search_name) {
        ui->le_upd_search_name->setMinimumWidth(380);
        ui->le_upd_search_name->setMaximumWidth(QWIDGETSIZE_MAX);
        ui->le_upd_search_name->setMinimumHeight(fieldHeight);
        ui->le_upd_search_name->setMaximumHeight(fieldHeight);
        ui->le_upd_search_name->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }

    const QList<QComboBox*> comboInputs = { ui->cb_mat_type, ui->cb_upd_type, ui->cb_sort_mat };
    for (auto *cb : comboInputs) {
        if (!cb) continue;
        cb->setMinimumHeight(fieldHeight);
        cb->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }

    // Barre supérieure: même look que Commandes (fond crème, marges, espacement)
    if (ui->top_nav) {
        ui->top_nav->setMinimumHeight(58);
        ui->top_nav->setStyleSheet("background-color: #fdf7f2; padding: 6px 10px;");
        if (auto *lay = ui->top_nav->layout()) {
            lay->setContentsMargins(10, 6, 10, 6);
            lay->setSpacing(12);
        }
    }
    if (ui->hbox_search_upd) {
        ui->hbox_search_upd->setSpacing(12);
        ui->hbox_search_upd->setStretch(0, 1); // la barre de recherche prend l'espace restant
        ui->hbox_search_upd->setStretch(1, 0); // le bouton conserve sa largeur minimale
        ui->hbox_search_upd->setContentsMargins(0, 0, 0, 0);
    }
    auto stylePrimaryBtn = [](QPushButton *btn){
        if (!btn) return;
        btn->setMinimumHeight(52);
        btn->setMaximumHeight(52);
        btn->setStyleSheet(
            "QPushButton{background:#8B4513;color:white;padding:12px 24px;border-radius:10px;font-weight:700;}"
            "QPushButton:hover{background:#a05a22;}"
            "QPushButton:pressed{background:#723a0f;}"
            );
    };

    if (ui->btn_upd_load) {
        ui->btn_upd_load->setMinimumWidth(220);
        ui->btn_upd_load->setMaximumWidth(220);
        ui->btn_upd_load->setMinimumHeight(fieldHeight);
        ui->btn_upd_load->setMaximumHeight(fieldHeight);
        ui->btn_upd_load->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        ui->btn_upd_load->show();
    }
    if (ui->btn_upd_confirm) {
        ui->btn_upd_confirm->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        stylePrimaryBtn(ui->btn_upd_confirm);
        ui->btn_upd_confirm->show();
    }

    // Boutons d'action principaux : même hauteur/couleur partout
    const QList<QPushButton*> primaryActions = {
        ui->btn_mat_add,
        ui->btn_upd_confirm,
        ui->btn_upd_load,
        ui->btn_export_pdf,
        ui->btn_show_stats
    };
    for (auto *btn : primaryActions) {
        stylePrimaryBtn(btn);
    }

    if (ui->hbox_stats_actions) {
        ui->hbox_stats_actions->setContentsMargins(0, 0, 0, 0);
        ui->hbox_stats_actions->setSpacing(0);
    }

    // Bouton suppression : hauteur alignée, couleur rouge conservée
    if (ui->btn_del_confirm) {
        ui->btn_del_confirm->setMinimumHeight(52);
        ui->btn_del_confirm->setMaximumHeight(52);
        ui->btn_del_confirm->setStyleSheet("QPushButton{background:#8B0000;color:white;padding:12px 24px;border-radius:10px;font-weight:700;}QPushButton:hover{background:#a51818;}QPushButton:pressed{background:#600000;}");
    }

    // Boutons de choix couleur : aligner la hauteur des barres
    const QList<QPushButton*> colorButtons = { ui->btn_color_add, ui->btn_color_upd };
    for (auto *btn : colorButtons) {
        if (!btn) continue;
        btn->setMinimumHeight(fieldHeight - 8);
        btn->setMaximumHeight(fieldHeight - 8);
        btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        btn->setMinimumWidth(190);
        btn->setMaximumWidth(220);
        stylePrimaryBtn(btn);
    }

    // Éviter l'écrasement de la ligne couleur (Add/Update)
    if (ui->le_mat_color) {
        ui->le_mat_color->setMinimumWidth(0);
        ui->le_mat_color->setMaximumWidth(QWIDGETSIZE_MAX);
        ui->le_mat_color->setMinimumHeight(fieldHeight);
        ui->le_mat_color->setMaximumHeight(fieldHeight);
        ui->le_mat_color->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }
    if (ui->le_upd_color) {
        ui->le_upd_color->setMinimumWidth(0);
        ui->le_upd_color->setMaximumWidth(QWIDGETSIZE_MAX);
        ui->le_upd_color->setMinimumHeight(fieldHeight);
        ui->le_upd_color->setMaximumHeight(fieldHeight);
        ui->le_upd_color->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }
    if (ui->colorLayoutAdd) {
        ui->colorLayoutAdd->setSpacing(12);
        ui->colorLayoutAdd->setStretch(0, 1);
        ui->colorLayoutAdd->setStretch(1, 0);
    }
    if (ui->colorLayoutUpd) {
        ui->colorLayoutUpd->setSpacing(12);
        ui->colorLayoutUpd->setStretch(0, 1);
        ui->colorLayoutUpd->setStretch(1, 0);
    }
    // Assurer la colonne champ qui s'étire dans les formulaires
    if (ui->formLayout_add) {
        ui->formLayout_add->setHorizontalSpacing(12);
        ui->formLayout_add->setVerticalSpacing(16);
        ui->formLayout_add->setRowWrapPolicy(QFormLayout::DontWrapRows);
        ui->formLayout_add->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        ui->formLayout_add->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->formLayout_add->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
        ui->formLayout_add->setItem(4, QFormLayout::FieldRole, ui->colorLayoutAdd);
        ui->formLayout_add->setItem(4, QFormLayout::LabelRole, ui->formLayout_add->itemAt(4, QFormLayout::LabelRole));
    }
    if (ui->formLayout_upd) {
        ui->formLayout_upd->setHorizontalSpacing(12);
        ui->formLayout_upd->setVerticalSpacing(16);
        ui->formLayout_upd->setRowWrapPolicy(QFormLayout::DontWrapRows);
        ui->formLayout_upd->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        ui->formLayout_upd->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->formLayout_upd->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
        ui->formLayout_upd->setItem(4, QFormLayout::FieldRole, ui->colorLayoutUpd);
        ui->formLayout_upd->setItem(4, QFormLayout::LabelRole, ui->formLayout_upd->itemAt(4, QFormLayout::LabelRole));
    }

    // Forcer la visibilité et les dimensions du bouton "CHARGER" en modification
    if (ui->btn_upd_load) {
        ui->btn_upd_load->setText("CHARGER");
        ui->btn_upd_load->setMinimumHeight(fieldHeight);
        ui->btn_upd_load->setMaximumHeight(fieldHeight);
        ui->btn_upd_load->setMinimumWidth(220);
        ui->btn_upd_load->setMaximumWidth(220);
        ui->btn_upd_load->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        ui->btn_upd_load->setStyleSheet(
            "QPushButton{background:#8B4513;color:white;border-radius:12px;padding:14px 26px;font-weight:700;}"
            "QPushButton:hover{background:#a05a22;}"
            "QPushButton:pressed{background:#723a0f;}"
            );
        ui->btn_upd_load->show();
    }

    // Désactiver les champs de modification tant qu'aucune matière n'est chargée
    const QList<QWidget*> updInputs = {
        ui->le_upd_name, ui->cb_upd_type, ui->dsb_upd_price, ui->sb_upd_qty,
        ui->le_upd_color, ui->dsb_upd_yield, ui->dsb_upd_loss,
        ui->btn_color_upd, ui->btn_upd_confirm
    };
    auto setUpdateEnabled = [updInputs](bool enabled) {
        for (auto *w : updInputs) {
            if (w) w->setEnabled(enabled);
        }
    };
    // Laisser le formulaire utilisable par défaut (sera rempli/validé via CHARGER)
    setUpdateEnabled(true);

    // Groupe exclusif pour les onglets supérieurs
    const QString tabStyle = "QPushButton{padding:10px 16px;border:1px solid #C68E65;border-radius:8px;background:#f7ede4;color:#3b2a20;font-weight:600;}"
                             "QPushButton:checked{background:#C68E65;color:white;}"
                             "QPushButton:hover{background:#e7d6c8;}";
    auto *navGroup = new QButtonGroup(this);
    navGroup->setExclusive(true);
    const QList<QPushButton*> navButtons = { ui->btn_tab_add, ui->btn_tab_update, ui->btn_tab_delete, ui->btn_tab_list, ui->btn_tab_stats };
    if (ui->btn_tab_add)    ui->btn_tab_add->setText("Ajouter une matière");
    if (ui->btn_tab_update) ui->btn_tab_update->setText("Modifier une matière");
    if (ui->btn_tab_delete) ui->btn_tab_delete->setText("Supprimer une matière");
    if (ui->btn_tab_list)   ui->btn_tab_list->setText("Liste des matières");
    if (ui->btn_tab_stats)  ui->btn_tab_stats->setText("Stats des matières");
    if (auto *layout = ui->top_nav) layout->setContentsMargins(10, 6, 10, 6);
    if (ui->btn_tab_add)    ui->btn_tab_add->setText("Ajouter une matière");
    if (ui->btn_tab_update) ui->btn_tab_update->setText("Modifier une matière");
    if (ui->btn_tab_delete) ui->btn_tab_delete->setText("Supprimer une matière");
    if (ui->btn_tab_list)   ui->btn_tab_list->setText("Liste des matières");
    if (ui->btn_tab_stats)  ui->btn_tab_stats->setText("Stats des matières");
    for (auto *btn : navButtons) {
        if (!btn) continue;
        navGroup->addButton(btn);
        btn->setCheckable(true);
        btn->setStyleSheet(tabStyle);
        btn->setMinimumHeight(44);
        btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    }
    if (ui->top_nav && ui->top_nav->layout()) {
        ui->top_nav->layout()->setContentsMargins(0, 0, 0, 0);
        ui->top_nav->layout()->setSpacing(10);
    }

    auto setNavChecked = [navButtons](QPushButton *active){
        for (auto *btn : navButtons) {
            if (!btn) continue;
            btn->setChecked(btn == active);
        }
    };

    // Par défaut, fenêtre indépendante. En mode intégré on masquera sidebar/top_nav

    // Ajuster la page de modification: centrer et limiter la largeur du formulaire
    if (ui->frame_upd && ui->page_update && ui->page_update->layout()) {
        ui->frame_upd->setMaximumWidth(QWIDGETSIZE_MAX); // occuper toute la largeur dispo
        ui->frame_upd->setMinimumWidth(0);
        ui->frame_upd->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        ui->page_update->layout()->setAlignment(ui->frame_upd, Qt::AlignTop);
        ui->page_update->layout()->setContentsMargins(10, 10, 10, 10);
        ui->page_update->layout()->setSpacing(16);
    }

    // Chargement et affichage du logo
    QPixmap logo;
    logo.load(":/images/logoo.png");
    QStringList logoCandidates;
    logoCandidates << "logoo.png"
                   << "logo.png"
                   << QCoreApplication::applicationDirPath() + "/logoo.png"
                   << QCoreApplication::applicationDirPath() + "/logo.png"
                   << QDir::currentPath() + "/logoo.png"
                   << QDir::currentPath() + "/logo.png";

    if (logo.isNull()) {
        for (const QString &path : logoCandidates) {
            if (logo.load(path)) {
                qDebug() << "Logo chargé depuis:" << path;
                break;
            }
        }
    }

    if (!logo.isNull()) {
        ui->logo_label->setMinimumHeight(220);
        ui->logo_label->setMaximumHeight(260);
        ui->logo_label->setPixmap(logo.scaled(220, 220, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        ui->logo_label->setAlignment(Qt::AlignCenter);
    } else {
        qDebug() << "Logo non trouvé. Ajoutez logo.png ou logoo.png à côté de l'exécutable.";
    }

    // Configuration du tableau
    ui->table_mat->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table_del_mat->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table_del_mat->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_del_mat->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_del_mat->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->table_del_mat->setAlternatingRowColors(true);
    ui->table_del_mat->setSortingEnabled(true);
    ui->table_del_mat->verticalHeader()->setVisible(false);

    // Rendre les valeurs des barres de progression lisibles
    auto setupProgressText = [](QProgressBar *bar) {
        if (!bar) return;
        bar->setTextVisible(true);
        bar->setAlignment(Qt::AlignCenter);
        bar->setStyleSheet(bar->styleSheet() + " color: white; font-weight: 700; font-size: 13px;");
    };
    setupProgressText(ui->pb_yield);
    setupProgressText(ui->pb_loss);

    // Cliquer une ligne remplit automatiquement l'ID à supprimer
    connect(ui->table_del_mat, &QTableWidget::itemSelectionChanged, this, [=]() {
        const int row = ui->table_del_mat->currentRow();
        if (row >= 0 && ui->table_del_mat->item(row, 0)) {
            ui->le_del_name->setText(ui->table_del_mat->item(row, 0)->text());
        }
    });

    // ouvrir la connexion via singleton
    if (Connection::instance()->createConnect()) {
        // (optionnel) création de table sur le serveur si nécessaire
        QSqlDatabase db = Connection::instance()->database();

        // Essayer de créer la séquence MATERIALS_SEQ si elle n'existe pas
        QSqlQuery seq(db);
        if (!seq.exec("CREATE SEQUENCE MATERIALS_SEQ START WITH 1 INCREMENT BY 1 NOCACHE")) {
            // La séquence existe probablement déjà, c'est OK
            qDebug() << "Sequence creation attempted:" << seq.lastError().text();
        }

        loadFromDatabase();
    } else {
        QMessageBox::warning(this, "Erreur base", "Impossible d'ouvrir la base de données.");
    }

    // --- LOGIQUE COULEUR (AJOUT) ---
    ui->le_mat_color->setReadOnly(true);
    ui->le_upd_color->setReadOnly(true);

    auto applyLeatherColor = [=](QLineEdit *lineEdit, QPushButton *button, const QString &label) {
        const QString colorHex = gLeatherColors.value(label, "#FFFFFF");
        lineEdit->setText(label);
        button->setStyleSheet(QString("background-color: %1; border: 1px solid #ccc; border-radius: 4px;").arg(colorHex));
    };

    auto chooseLeatherColor = [=](QLineEdit *lineEdit, QPushButton *button) {
        QDialog dlg(this);
        dlg.setWindowTitle("Table des couleurs cuir");
        dlg.resize(420, 320);

        auto *layout = new QVBoxLayout(&dlg);
        auto *table = new QTableWidget(&dlg);
        table->setColumnCount(2);
        table->setHorizontalHeaderLabels({"Couleur", "Apercu"});
        table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);

        QStringList labels = gLeatherColors.keys();
        table->setRowCount(labels.size());
        for (int row = 0; row < labels.size(); ++row) {
            const QString label = labels.at(row);
            const QString hex = gLeatherColors.value(label);

            auto *nameItem = new QTableWidgetItem(label);
            table->setItem(row, 0, nameItem);

            auto *previewItem = new QTableWidgetItem(hex);
            previewItem->setBackground(QColor(hex));
            table->setItem(row, 1, previewItem);
        }

        int selectedRow = labels.indexOf(lineEdit->text());
        if (selectedRow < 0) {
            selectedRow = labels.indexOf("Cognac");
        }
        if (selectedRow < 0) {
            selectedRow = 0;
        }
        table->selectRow(selectedRow);

        QObject::connect(table, &QTableWidget::cellDoubleClicked, &dlg, &QDialog::accept);

        auto *btnLayout = new QHBoxLayout();
        auto *btnCancel = new QPushButton("Annuler", &dlg);
        auto *btnSelect = new QPushButton("Choisir", &dlg);
        btnLayout->addStretch();
        btnLayout->addWidget(btnCancel);
        btnLayout->addWidget(btnSelect);

        QObject::connect(btnCancel, &QPushButton::clicked, &dlg, &QDialog::reject);
        QObject::connect(btnSelect, &QPushButton::clicked, &dlg, &QDialog::accept);

        layout->addWidget(table);
        layout->addLayout(btnLayout);

        if (dlg.exec() == QDialog::Accepted && table->currentRow() >= 0) {
            const QString choice = table->item(table->currentRow(), 0)->text();
            applyLeatherColor(lineEdit, button, choice);
        }
    };

    // Couleur par defaut pour ajout/modification
    applyLeatherColor(ui->le_mat_color, ui->btn_color_add, "Cognac");
    applyLeatherColor(ui->le_upd_color, ui->btn_color_upd, "Cognac");
    ui->btn_color_add->setToolTip("Ouvrir le tableau des couleurs cuir");
    ui->btn_color_upd->setToolTip("Ouvrir le tableau des couleurs cuir");

    // Controle de saisie: ID strictement numerique et positif
    auto *idValidator = new QIntValidator(1, 999999999, this);
    ui->le_mat_name->setValidator(idValidator);
    ui->le_upd_search_name->setValidator(idValidator);
    ui->le_upd_name->setValidator(idValidator);
    ui->le_del_name->setValidator(idValidator);

    auto validateMaterialInput = [=](const QString &idText,
                                     double price,
                                     int quantity,
                                     double yield,
                                     double loss,
                                     const QString &colorLabel,
                                     bool requirePaletteColor) -> bool {
        bool ok = false;
        int id = idText.toInt(&ok);
        if (!ok || id <= 0) {
            QMessageBox::warning(this, "Erreur", "ID matière invalide.");
            return false;
        }
        if (price <= 0.0) {
            QMessageBox::warning(this, "Erreur", "Le prix doit être supérieur à 0.");
            return false;
        }
        if (quantity < 0) {
            QMessageBox::warning(this, "Erreur", "La quantité ne peut pas être négative.");
            return false;
        }
        if (yield < 0.0 || yield > 100.0 || loss < 0.0 || loss > 100.0) {
            QMessageBox::warning(this, "Erreur", "Rendement et perte doivent être entre 0 et 100.");
            return false;
        }
        if ((yield + loss) > 100.0) {
            QMessageBox::warning(this, "Erreur", "Rendement + perte ne doit pas dépasser 100%.");
            return false;
        }
        if (colorLabel.trimmed().isEmpty()) {
            QMessageBox::warning(this, "Erreur", "La couleur ne peut pas être vide.");
            return false;
        }
        if (requirePaletteColor && !gLeatherColors.contains(colorLabel)) {
            QMessageBox::warning(this, "Erreur", "Choisissez une couleur cuir depuis le tableau.");
            return false;
        }
        return true;
    };

    connect(ui->btn_color_add, &QPushButton::clicked, this, [=]() {
        chooseLeatherColor(ui->le_mat_color, ui->btn_color_add);
    });

    // --- LOGIQUE COULEUR (MODIFICATION) ---
    connect(ui->btn_color_upd, &QPushButton::clicked, this, [=]() {
        chooseLeatherColor(ui->le_upd_color, ui->btn_color_upd);
    });

    // --- FONCTIONNALITÉS CRUD ---

    // 1. Ajouter
    connect(ui->btn_mat_add, &QPushButton::clicked, this, [=]() {
        const QString matType = ui->cb_mat_type->currentText().trimmed();
        if (matType.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez choisir un type de matière.");
            ui->cb_mat_type->setFocus();
            return;
        }
        if (!validateMaterialInput(ui->le_mat_name->text(),
                                   ui->dsb_mat_price->value(),
                                   ui->sb_mat_qty->value(),
                                   ui->dsb_mat_yield->value(),
                                   ui->dsb_mat_loss->value(),
                                   ui->le_mat_color->text(),
                                   /*requirePaletteColor=*/true)) {
            return;
        }

        QString matNameValue = matType;

        bool idOk = false;
        int idValue = ui->le_mat_name->text().toInt(&idOk);

        // enregistrer dans la base Oracle
        QSqlDatabase db = Connection::instance()->database();
        if (!db.isOpen()) {
            if (!Connection::instance()->createConnect()) {
                QMessageBox::critical(this, "Erreur", "La connexion à la base de données est perdue.");
                return;
            }
            db = Connection::instance()->database();
        }

        // Controle d'unicite de l'ID
        QSqlQuery checkQuery(db);
        checkQuery.prepare(QString("SELECT COUNT(1) FROM %1 WHERE id_matiere=?").arg(gMaterialsTableName));
        checkQuery.addBindValue(idValue);
        if (!checkQuery.exec() || !checkQuery.next()) {
            QMessageBox::critical(this, "Erreur", "Impossible de vérifier l'unicité de l'ID.");
            return;
        }
        if (checkQuery.value(0).toInt() > 0) {
            QMessageBox::warning(this, "Erreur", "Cet ID matière existe déjà.");
            return;
        }

        QSqlQuery ins(db);
        if (gHasNomMatiere) {
            ins.prepare(QString("INSERT INTO %1 (id_matiere, nom_matiere, type_matiere, prix, quantite_stock, couleur, rendement, perte_matiere) VALUES (?, ?, ?, ?, ?, ?, ?, ?)" ).arg(gMaterialsTableName));
            ins.addBindValue(idValue);
            ins.addBindValue(matNameValue);
            ins.addBindValue(ui->cb_mat_type->currentText());
            ins.addBindValue(ui->dsb_mat_price->value());
            ins.addBindValue(ui->sb_mat_qty->value());
            ins.addBindValue(ui->le_mat_color->text());
            ins.addBindValue(ui->dsb_mat_yield->value());
            ins.addBindValue(ui->dsb_mat_loss->value());
        } else {
            ins.prepare(QString("INSERT INTO %1 (id_matiere, type_matiere, prix, quantite_stock, couleur, rendement, perte_matiere) VALUES (?, ?, ?, ?, ?, ?, ?)" ).arg(gMaterialsTableName));
            ins.addBindValue(idValue);
            ins.addBindValue(ui->cb_mat_type->currentText());
            ins.addBindValue(ui->dsb_mat_price->value());
            ins.addBindValue(ui->sb_mat_qty->value());
            ins.addBindValue(ui->le_mat_color->text());
            ins.addBindValue(ui->dsb_mat_yield->value());
            ins.addBindValue(ui->dsb_mat_loss->value());
        }

        if(!ins.exec()) {
            QString errorMsg = "Erreur lors de l'insertion : " + ins.lastError().text();
            qDebug() << "Insert failed:" << errorMsg;
            QMessageBox::critical(this, "Erreur Insertion", errorMsg);
            return;
        }

        // Commiter la transaction pour Oracle
        if (!db.commit()) {
            qDebug() << "Commit failed:" << db.lastError().text();
            QMessageBox::critical(this, "Erreur", "Impossible de valider l'enregistrement.");
            db.rollback();
            return;
        }

        // mettre à jour la table affichée
        loadFromDatabase();

        // Innovation : Notification Rupture immédiate
        if(ui->sb_mat_qty->value() < 10) {
            QMessageBox::warning(this, "Alerte Stock", "Attention : Le stock de " + ui->le_mat_name->text() + " est critique !");
        }

        QMessageBox::information(this, "Succès", "Matière ajoutée avec succès et affichée dans la liste.");

        // Reset des champs
        ui->le_mat_name->clear();
        ui->le_mat_color->setText("Cognac");
        ui->sb_mat_qty->setValue(0);
        ui->dsb_mat_price->setValue(0);
        ui->dsb_mat_yield->setValue(0);
        ui->dsb_mat_loss->setValue(0);
        ui->btn_color_add->setStyleSheet("background-color: #9A5B2D; border: 1px solid #ccc; border-radius: 4px;");
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

    // Export PDF de la liste des matières
    connect(ui->btn_export_pdf, &QPushButton::clicked, this, [=]() {
        if (ui->table_mat->rowCount() == 0) {
            QMessageBox::warning(this, "Export PDF", "Aucune matière à exporter.");
            return;
        }

        QString defaultPath = QDir::homePath() + "/matieres_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".pdf";
        QString filePath = QFileDialog::getSaveFileName(this, "Exporter en PDF", defaultPath, "PDF Files (*.pdf)");
        if (filePath.isEmpty()) {
            return;
        }
        if (!filePath.endsWith(".pdf", Qt::CaseInsensitive)) {
            filePath += ".pdf";
        }

        QPdfWriter pdf(filePath);
        pdf.setPageSize(QPageSize(QPageSize::A4));
        pdf.setPageOrientation(QPageLayout::Landscape);
        pdf.setResolution(150);

        QPainter painter(&pdf);
        if (!painter.isActive()) {
            QMessageBox::critical(this, "Export PDF", "Impossible de créer le fichier PDF.");
            return;
        }

        const int margin = 40;
        const int rowHeight = 28;
        int y = margin;

        QFont titleFont("Arial", 12, QFont::Bold);
        painter.setFont(titleFont);
        painter.drawText(margin, y, "Liste des Matières Premières");
        y += 24;

        QFont infoFont("Arial", 8);
        painter.setFont(infoFont);
        painter.drawText(margin, y, "Exporté le: " + QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm"));
        y += 20;

        const int colCount = ui->table_mat->columnCount();
        const int usableWidth = pdf.width() - (2 * margin);
        const int colWidth = usableWidth / qMax(1, colCount);

        auto drawHeader = [&]() {
            QFont headerFont("Arial", 8, QFont::Bold);
            painter.setFont(headerFont);
            for (int c = 0; c < colCount; ++c) {
                const int x = margin + (c * colWidth);
                painter.drawRect(x, y, colWidth, rowHeight);
                QTableWidgetItem *hItem = ui->table_mat->horizontalHeaderItem(c);
                painter.drawText(x + 4, y, colWidth - 8, rowHeight, Qt::AlignCenter, hItem ? hItem->text() : QString("Col %1").arg(c + 1));
            }
            y += rowHeight;
        };

        drawHeader();

        QFont bodyFont("Arial", 8);
        painter.setFont(bodyFont);

        for (int r = 0; r < ui->table_mat->rowCount(); ++r) {
            if (ui->table_mat->isRowHidden(r)) {
                continue;
            }

            if (y + rowHeight > (pdf.height() - margin)) {
                pdf.newPage();
                y = margin;
                drawHeader();
                painter.setFont(bodyFont);
            }

            for (int c = 0; c < colCount; ++c) {
                const int x = margin + (c * colWidth);
                painter.drawRect(x, y, colWidth, rowHeight);
                QTableWidgetItem *item = ui->table_mat->item(r, c);
                painter.drawText(x + 4, y, colWidth - 8, rowHeight, Qt::AlignVCenter | Qt::AlignLeft, item ? item->text() : "");
            }
            y += rowHeight;
        }

        painter.end();
        QMessageBox::information(this, "Export PDF", "PDF exporté avec succès:\n" + filePath);
    });

    // 3. Charger pour Modifier (lecture directe DB pour fiabilité)
    connect(ui->btn_upd_load, &QPushButton::clicked, this, [=]() {
        const QString name = ui->le_upd_search_name->text().trimmed();
        if (name.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Entrez l'ID de la matière à charger.");
            ui->le_upd_search_name->setFocus();
            return;
        }
        bool okNum = false;
        const int searchId = name.toInt(&okNum);
        if (!okNum || searchId <= 0) {
            QMessageBox::warning(this, "Erreur", "L'ID doit être numérique et positif.");
            ui->le_upd_search_name->setFocus();
            return;
        }

        // Assurer la connexion
        QSqlDatabase db = Connection::instance()->database();
        if (!db.isOpen()) {
            if (!Connection::instance()->createConnect()) {
                QMessageBox::critical(this, "Erreur", "Connexion base indisponible.");
                return;
            }
            db = Connection::instance()->database();
        }

        QSqlQuery q(db);
        q.prepare(QString(
                      "SELECT id_matiere, NVL(nom_matiere, type_matiere) AS nom_matiere, type_matiere, prix, quantite_stock, couleur, rendement, perte_matiere "
                      "FROM %1 WHERE id_matiere=?"
                      ).arg(gMaterialsTableName));
        q.addBindValue(searchId);

        if (!q.exec()) {
            QMessageBox::critical(this, "Erreur", "Échec de la requête de chargement:\n" + q.lastError().text());
            return;
        }

        if (!q.next()) {
            setUpdateEnabled(true); // laisser éditable pour retenter
            ui->le_upd_name->clear();
            ui->cb_upd_type->setCurrentIndex(0);
            ui->dsb_upd_price->setValue(0);
            ui->sb_upd_qty->setValue(0);
            ui->le_upd_color->clear();
            ui->dsb_upd_yield->setValue(0);
            ui->dsb_upd_loss->setValue(0);
            ui->btn_color_upd->setStyleSheet("background-color: white; border: 1px solid #ccc;");
            QMessageBox::warning(this, "Erreur", "Matière introuvable.");
            return;
        }

        // Remplir les champs depuis la base (type réel inclus)
        ui->le_upd_name->setText(q.value(0).toString());
        const QString typeVal = q.value(2).toString();
        if (!typeVal.isEmpty()) {
            ui->cb_upd_type->setCurrentText(typeVal);
        } else {
            ui->cb_upd_type->setCurrentIndex(0);
        }
        ui->dsb_upd_price->setValue(q.value(3).toDouble());
        ui->sb_upd_qty->setValue(q.value(4).toInt());
        const QString colorText = q.value(5).toString();
        ui->le_upd_color->setText(colorText);
        const QString colorHex = gLeatherColors.value(colorText, colorText);
        ui->btn_color_upd->setStyleSheet(QString("background-color: %1; border: 1px solid #ccc; border-radius: 4px;").arg(colorHex));
        ui->dsb_upd_yield->setValue(q.value(6).toDouble());
        ui->dsb_upd_loss->setValue(q.value(7).toDouble());

        // Activer le formulaire une fois l'ID trouvé
        setUpdateEnabled(true);
        ui->le_upd_name->setReadOnly(true);
    });

    // 4. Confirmer Modification
    connect(ui->btn_upd_confirm, &QPushButton::clicked, this, [=]() {
        QString searchName = ui->le_upd_search_name->text().trimmed();
        if (searchName.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez d'abord charger une matiere.");
            return;
        }

        const QString matType = ui->cb_upd_type->currentText().trimmed();
        if (matType.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez choisir un type de matière.");
            ui->cb_upd_type->setFocus();
            return;
        }

        // En modification, on accepte aussi les anciennes couleurs libres (palette non obligatoire)
        if (!validateMaterialInput(ui->le_upd_name->text(),
                                   ui->dsb_upd_price->value(),
                                   ui->sb_upd_qty->value(),
                                   ui->dsb_upd_yield->value(),
                                   ui->dsb_upd_loss->value(),
                                   ui->le_upd_color->text(),
                                   /*requirePaletteColor=*/false)) {
            return;
        }

        bool idOk = false;
        int newIdValue = ui->le_upd_name->text().toInt(&idOk);
        if (!idOk) {
            QMessageBox::warning(this, "Erreur", "L'ID doit etre numerique.");
            return;
        }

        bool searchOk = false;
        int searchIdValue = searchName.toInt(&searchOk);
        if (!searchOk) {
            QMessageBox::warning(this, "Erreur", "L'ID a modifier doit etre numerique.");
            return;
        }

        // Update dans la base Oracle
        QSqlDatabase db = Connection::instance()->database();
        if (!db.isOpen()) {
            if (!Connection::instance()->createConnect()) {
                QMessageBox::critical(this, "Erreur", "Connexion base perdue pendant la modification.");
                return;
            }
            db = Connection::instance()->database();
        }

        if (!db.transaction()) {
            qDebug() << "Transaction start failed:" << db.lastError().text();
        }

        QSqlQuery upd(db);
        if (gHasNomMatiere) {
            upd.prepare(QString("UPDATE %1 SET id_matiere=?, nom_matiere=?, type_matiere=?, prix=?, quantite_stock=?, couleur=?, rendement=?, perte_matiere=? WHERE id_matiere=?").arg(gMaterialsTableName));
            upd.addBindValue(newIdValue);
            upd.addBindValue(ui->cb_upd_type->currentText()); // nom_matiere (placeholder: type as name)
            upd.addBindValue(ui->cb_upd_type->currentText()); // type_matiere
            upd.addBindValue(ui->dsb_upd_price->value());
            upd.addBindValue(ui->sb_upd_qty->value());
            upd.addBindValue(ui->le_upd_color->text());
            upd.addBindValue(ui->dsb_upd_yield->value());
            upd.addBindValue(ui->dsb_upd_loss->value());
            upd.addBindValue(searchIdValue);
        } else {
            upd.prepare(QString("UPDATE %1 SET id_matiere=?, type_matiere=?, prix=?, quantite_stock=?, couleur=?, rendement=?, perte_matiere=? WHERE id_matiere=?").arg(gMaterialsTableName));
            upd.addBindValue(newIdValue);
            upd.addBindValue(ui->cb_upd_type->currentText());
            upd.addBindValue(ui->dsb_upd_price->value());
            upd.addBindValue(ui->sb_upd_qty->value());
            upd.addBindValue(ui->le_upd_color->text());
            upd.addBindValue(ui->dsb_upd_yield->value());
            upd.addBindValue(ui->dsb_upd_loss->value());
            upd.addBindValue(searchIdValue);
        }

        if(!upd.exec()) {
            QString sqlErr = upd.lastError().text();
            qDebug() << "Update failed:" << sqlErr;
            db.rollback();
            QMessageBox::critical(this, "Erreur", "Échec de la modification en base:\n" + sqlErr);
            return;
        }

        if (upd.numRowsAffected() == 0) {
            QMessageBox::warning(this, "Erreur", "Aucune matière trouvée pour cet ID.");
            return;
        }

        // Commiter la transaction pour Oracle
        if (!db.commit()) {
            QString commitErr = db.lastError().text();
            qDebug() << "Commit failed:" << commitErr;
            QMessageBox::critical(this, "Erreur", "Impossible de valider la modification.");
            db.rollback();
            return;
        }

        // Recharger la table
        loadFromDatabase();
        QMessageBox::information(this, "Succès", "Matière modifiée avec succès.");
        ui->le_upd_search_name->clear();
        ui->le_upd_name->clear();
        ui->dsb_upd_price->setValue(0);
        ui->sb_upd_qty->setValue(0);
        ui->le_upd_color->clear();
        ui->dsb_upd_yield->setValue(0);
        ui->dsb_upd_loss->setValue(0);
        // Laisser le formulaire actif pour enchaîner une nouvelle modification
        setUpdateEnabled(true);
    });

    // 5. Supprimer
    connect(ui->btn_del_confirm, &QPushButton::clicked, this, [=]() {
        QString name = ui->le_del_name->text();
        if (name.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Entrez l'ID de la matiere a supprimer.");
            return;
        }

        bool idOk = false;
        int idValue = name.toInt(&idOk);
        if (!idOk) {
            QMessageBox::warning(this, "Erreur", "L'ID doit etre numerique.");
            return;
        }
        if (idValue <= 0) {
            QMessageBox::warning(this, "Erreur", "ID matière invalide.");
            return;
        }

        // Confirmation avant suppression
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmer suppression",
                                                                  "Êtes-vous sûr de vouloir supprimer '" + name + "' ?",
                                                                  QMessageBox::Yes | QMessageBox::No);
        if (reply != QMessageBox::Yes) return;

        // Delete de la base Oracle
        QSqlDatabase db = Connection::instance()->database();
        if (!db.transaction()) {
            qDebug() << "Transaction start failed (delete):" << db.lastError().text();
        }

        // Supprimer les lignes enfants (FK_FOURNIR_MAT) avant la suppression principale
        cleanupChildConstraintRows(db, idValue);

        QSqlQuery del(db);
        del.prepare(QString("DELETE FROM %1 WHERE id_matiere=?").arg(gMaterialsTableName));
        del.addBindValue(idValue);

        if(!del.exec()) {
            const QString sqlErr = del.lastError().text();
            qDebug() << "Delete failed:" << sqlErr;
            QMessageBox::critical(this, "Erreur", "Échec de la suppression en base (vérifiez les dépendances fournisseurs, etc.):\n" + sqlErr);
            db.rollback();
            return;
        }

        // Commiter la transaction pour Oracle
        if (!db.commit()) {
            qDebug() << "Commit failed:" << db.lastError().text();
            QMessageBox::critical(this, "Erreur", "Impossible de valider la suppression.");
            db.rollback();
            return;
        }

        // Vérifier si au moins une ligne a été supprimée
        if (del.numRowsAffected() == 0) {
            QMessageBox::warning(this, "Erreur", "Matière introuvable.");
            return;
        }

        // Recharger la table
        loadFromDatabase();
        QMessageBox::information(this, "Succès", "Matière supprimée avec succès.");
        ui->le_del_name->clear();
    });

    // --- NAVIGATION ---
    connect(ui->btn_tab_add, &QPushButton::clicked, this, [=](){ setNavChecked(ui->btn_tab_add); ui->stackedWidget->setCurrentWidget(ui->page_add); });
    connect(ui->btn_tab_update, &QPushButton::clicked, this, [=](){ setNavChecked(ui->btn_tab_update); ui->stackedWidget->setCurrentWidget(ui->page_update); });
    connect(ui->btn_tab_delete, &QPushButton::clicked, this, [=](){ setNavChecked(ui->btn_tab_delete); ui->stackedWidget->setCurrentWidget(ui->page_delete); });
    connect(ui->btn_tab_list, &QPushButton::clicked, this, [=](){ setNavChecked(ui->btn_tab_list); ui->stackedWidget->setCurrentWidget(ui->page_list); });
    connect(ui->btn_tab_stats, &QPushButton::clicked, this, [=](){
        refreshStats();
        setNavChecked(ui->btn_tab_stats);
        ui->stackedWidget->setCurrentWidget(ui->page_stats);
    });

    // Connexion du bouton latéral "Matières premières"
    connect(ui->btn_nav_material, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentWidget(ui->page_list);
    });

    // Configuration par défaut
    setNavChecked(ui->btn_tab_add);
    ui->stackedWidget->setCurrentWidget(ui->page_add);
    ui->factory_title->setText("Leather Factory Management");

    // --- STATISTIQUES ---
    connect(ui->btn_show_stats, &QPushButton::clicked, this, [=]() {
        refreshStats();
        setNavChecked(ui->btn_tab_stats);
        ui->stackedWidget->setCurrentWidget(ui->page_stats);
    });
}

static bool tableHasColumn(QSqlDatabase &db, const QString &tableName, const QString &columnName)
{
    QSqlRecord rec = db.record(tableName);
    return rec.indexOf(columnName) >= 0;
}

static void cleanupChildConstraintRows(QSqlDatabase &db, int idValue)
{
    // Supprime les lignes enfants liées via la contrainte FK_FOURNIR_MAT (schéma rayen)
    QSqlQuery fk(db);
    if (fk.exec("SELECT table_name, column_name FROM user_cons_columns WHERE constraint_name = 'FK_FOURNIR_MAT'")) {
        while (fk.next()) {
            const QString childTable = fk.value(0).toString();
            const QString childCol = fk.value(1).toString();
            if (childTable.isEmpty() || childCol.isEmpty()) continue;

            QSqlQuery cleanup(db);
            cleanup.prepare(QString("DELETE FROM %1 WHERE %2 = ?").arg(childTable, childCol));
            cleanup.addBindValue(idValue);
            if (!cleanup.exec()) {
                qDebug() << "Child delete failed on" << childTable << childCol << cleanup.lastError().text();
            } else {
                qDebug() << "Child rows removed from" << childTable << "via" << childCol << "for" << idValue;
            }
        }
    } else {
        qDebug() << "FK lookup failed:" << fk.lastError().text();
    }

    // Fallback si la contrainte n'est pas trouvée : tenter directement
    QStringList childTables;
    childTables << "FOURNIR_MAT" << "RAYEN.FOURNIR_MAT" << "\"RAYEN\".\"FOURNIR_MAT\"";
    QStringList childIdCols;
    childIdCols << "ID_MATIERE" << "ID_MAT" << "ID_MATERIEL" << "ID_MATIERE_PREMIERE";

    for (const QString &child : childTables) {
        for (const QString &col : childIdCols) {
            QSqlQuery cleanup(db);
            cleanup.prepare(QString("DELETE FROM %1 WHERE %2=?").arg(child, col));
            cleanup.addBindValue(idValue);
            if (!cleanup.exec()) {
                qDebug() << "Child delete fallback failed on" << child << col << cleanup.lastError().text();
            }
        }
    }
}

void MaterialsWindow::setEmbeddedMode(bool embedded)
{
    embeddedMode = embedded;
    if (embedded && ui) {
        // Masquer uniquement la sidebar pour éviter le doublon avec le menu principal
        if (ui->sidebar) ui->sidebar->hide();
        if (ui->top_nav) { ui->top_nav->show(); ui->top_nav->raise(); }

        // Supprimer les marges pour occuper davantage l'espace disponible
        if (ui->centralwidget) ui->centralwidget->setContentsMargins(0, 0, 0, 0);
        if (ui->stackedWidget) {
            // Laisser un léger padding interne pour toutes les pages (Ajouter/Liste/Stats)
            ui->stackedWidget->setContentsMargins(10, 0, 10, 10);
            ui->stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        }

        adjustEmbeddedLayout();
    }
}

void MaterialsWindow::adjustEmbeddedLayout()
{
    if (!embeddedMode || !ui || !ui->stackedWidget || !ui->centralwidget) {
        return;
    }

    // Si un layout gère déjà le dimensionnement, éviter les setGeometry agressifs
    if (ui->centralwidget->layout()) {
        ui->stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        if (ui->top_nav) ui->top_nav->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        return;
    }

    const QSize size = ui->centralwidget->size();
    const int width = size.width();

    int topOffset = 0;
    if (ui->top_nav && ui->top_nav->isVisible()) {
        const int navHeight = ui->top_nav->sizeHint().height();
        // Appliquer un léger padding latéral pour rappeler la page Commandes
        const int hMargin = 10;
        ui->top_nav->setGeometry(hMargin, 0, qMax(0, width - 2 * hMargin), navHeight);
        topOffset = navHeight + 6; // petit espace entre le bandeau et le contenu
    }

    const int height = size.height() - topOffset;
    const int hMargin = 10;
    ui->stackedWidget->setGeometry(hMargin, topOffset, qMax(0, width - 2 * hMargin), qMax(0, height));
}

void MaterialsWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    adjustEmbeddedLayout();
}

// helper method calculates and updates statistic widgets
void MaterialsWindow::refreshStats()
{
    int total = ui->table_mat->rowCount();
    ui->lbl_total_mat_val->setText(QString::number(total));

    int lowStockCount = 0;
    double totalYield = 0;
    double totalLoss = 0;
    QString wasteAdvice = "Niveau de perte acceptable.";
    QMap<QString, int> typeCounts;

    for(int i=0; i<total; ++i) {
        if(ui->table_mat->item(i, 3)->text().toInt() < 10) lowStockCount++;
        totalYield += ui->table_mat->item(i, 5)->text().toDouble();
        totalLoss += ui->table_mat->item(i, 6)->text().toDouble();
        QString typeVal = ui->table_mat->item(i, 1)->text().trimmed();
        if (typeVal.isEmpty()) {
            typeVal = "Inconnu";
        }
        typeCounts[typeVal] += 1;
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

    const double avgYieldVal = (total > 0) ? qBound(0.0, totalYield / total, 100.0) : 0.0;
    const double avgLossVal = qBound(0.0, avgLoss, 100.0);

    ui->pb_yield->setRange(0, 100);
    ui->pb_loss->setRange(0, 100);
    ui->pb_yield->setValue(static_cast<int>(avgYieldVal));
    ui->pb_loss->setValue(static_cast<int>(avgLossVal));

    // Afficher les pourcentages sur les barres
    ui->pb_yield->setFormat(QString::number(avgYieldVal, 'f', 1) + " %");
    ui->pb_loss->setFormat(QString::number(avgLossVal, 'f', 1) + " %");

    // Pie chart: repartition par type de matiere
    if (ui->chart_mat_type) {
        auto *series = new QPieSeries();
        if (typeCounts.isEmpty()) {
            series->append("Aucune donnee", 1);
        } else {
            for (auto it = typeCounts.constBegin(); it != typeCounts.constEnd(); ++it) {
                series->append(it.key(), it.value());
            }
        }

        auto *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Repartition par type");
        chart->legend()->setAlignment(Qt::AlignRight);
        chart->setBackgroundVisible(false);

        if (ui->chart_mat_type->chart()) {
            ui->chart_mat_type->chart()->deleteLater();
        }
        ui->chart_mat_type->setChart(chart);
        ui->chart_mat_type->setRenderHint(QPainter::Antialiasing);
    }
}


void MaterialsWindow::loadFromDatabase()
{
    qDebug() << "loadFromDatabase start";
    QSqlDatabase db = Connection::instance()->database();
    if (!db.isOpen()) {
        qDebug() << "Database is not open; attempting reconnect...";
        if (!Connection::instance()->createConnect()) {
            QMessageBox::critical(this, "Erreur", "Base de données non connectée.");
            return;
        }
        db = Connection::instance()->database();
    }

    QSqlQuery query(db);
    QStringList candidates;
    candidates << gMaterialsTableName
               << "MATIERE_PREMIERE"
               << "RAYEN.MATIERE_PREMIERE"
               << "\"RAYEN\".\"MATIERE_PREMIERE\""
               << "MARYEM.MATIERE_PREMIERE"
               << "\"MARYEM\".\"MATIERE_PREMIERE\""
               << "MATERIALS"
               << "RAYEN.MATERIALS"
               << "\"RAYEN\".\"MATERIALS\""
               << "MARYEM.MATERIALS"
               << "\"MARYEM\".\"MATERIALS\"";
    bool foundTable = false;
    QString lastError;

    for (const QString &candidate : candidates) {
        const QString sql = QString(
                                "SELECT id_matiere, NVL(nom_matiere, type_matiere) AS nom_matiere, prix, quantite_stock, couleur, rendement, perte_matiere FROM %1"
                                ).arg(candidate);
        if (query.exec(sql)) {
            gMaterialsTableName = candidate;
            gHasNomMatiere = tableHasColumn(db, candidate, "NOM_MATIERE");
            foundTable = true;
            break;
        }
        lastError = query.lastError().text();
    }

    if (!foundTable) {
        QString errorMsg = "Erreur requête : " + lastError;
        qDebug() << "Query failed:" << errorMsg;
        QMessageBox::critical(this, "Erreur SELECT", errorMsg + "\n\nVérifiez que la table MATIERE_PREMIERE (ou MATERIALS) existe dans le schéma Oracle.");
        return;
    }

    ui->table_mat->setRowCount(0);
    const bool delSorting = ui->table_del_mat->isSortingEnabled();
    ui->table_del_mat->setSortingEnabled(false);
    ui->table_del_mat->setRowCount(0);
    int row = 0;
    const int fieldCount = query.record().count();
    const int maxCols = qMin(7, fieldCount);
    while(query.next()) {
        ui->table_mat->insertRow(row);
        for(int col = 0; col < maxCols; ++col) {
            ui->table_mat->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }

        ui->table_del_mat->insertRow(row);
        for (int col = 0; col < maxCols; ++col) {
            ui->table_del_mat->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }

        ++row;
    }
    ui->table_del_mat->setSortingEnabled(delSorting);
    qDebug() << "Table loaded with" << row << "rows";
    qDebug() << "loadFromDatabase end";
}

MaterialsWindow::~MaterialsWindow()
{
    delete ui;
}
