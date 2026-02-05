/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QFrame *sideFrame;
    QVBoxLayout *verticalLayout_side;
    QPushButton *clientsButton;
    QPushButton *commandsButton;
    QPushButton *stocksButton;
    QSpacerItem *verticalSpacer;
    QPushButton *logoutButton;
    QTabWidget *tabWidget;
    QWidget *tabHome;
    QHBoxLayout *horizontalLayout_main;
    QGroupBox *groupAdd;
    QFormLayout *formLayout;
    QLabel *labelSKU;
    QLineEdit *lineSKU;
    QLabel *labelName;
    QLineEdit *lineName;
    QLabel *labelCategory;
    QComboBox *comboCategory;
    QLabel *labelBrand;
    QComboBox *comboBrand;
    QLabel *labelPurchase;
    QLineEdit *linePurchase;
    QLabel *labelSelling;
    QLineEdit *lineSelling;
    QLabel *labelStock;
    QSpinBox *spinStock;
    QLabel *labelLocation;
    QLineEdit *lineLocation;
    QHBoxLayout *horizontalLayout_actions;
    QPushButton *confirmButton;
    QPushButton *cancelButton;
    QGroupBox *groupList;
    QVBoxLayout *verticalLayout_list;
    QLineEdit *searchLine;
    QTableWidget *tableArticles;
    QHBoxLayout *horizontalLayout_tableActions;
    QPushButton *exportButton;
    QPushButton *deleteButton;
    QWidget *tabMatieres;
    QHBoxLayout *horizontalLayout_matieres;
    QGroupBox *groupAddMatiere;
    QFormLayout *formLayout_matiere;
    QLabel *labelMatiereName;
    QLineEdit *lineMatiereName;
    QLabel *labelMatiereType;
    QComboBox *comboMatiereType;
    QLabel *labelMatiereQuantite;
    QSpinBox *spinMatiereQuantite;
    QLabel *labelMatiereCouleur;
    QLineEdit *lineMatiereColor;
    QLabel *labelMatierePerte;
    QLineEdit *lineMatiereWaste;
    QLabel *labelMatiereStatut;
    QComboBox *comboMatiereStatus;
    QHBoxLayout *horizontalLayout_matiereActions;
    QPushButton *confirmMatierButton;
    QPushButton *cancelMatierButton;
    QTimeEdit *timeEdit;
    QGroupBox *groupListMatieres;
    QVBoxLayout *verticalLayout_listMatieres;
    QLineEdit *searchMatieres;
    QHBoxLayout *horizontalLayout_filterMatieres;
    QLabel *labelFilterStatus;
    QComboBox *comboFilterStatus;
    QTableWidget *tableMatieres;
    QHBoxLayout *horizontalLayout_matieresActions;
    QPushButton *statisticsButton;
    QPushButton *exportMatiereButton;
    QPushButton *deleteMatierButton;
    QWidget *tabCommands;
    QHBoxLayout *horizontalLayout_commands;
    QGroupBox *groupAddCommand;
    QFormLayout *formLayout_command;
    QLabel *labelCommandID;
    QLineEdit *lineCommandID;
    QLabel *labelCommandType;
    QComboBox *comboCommandType;
    QLabel *labelCommandSeller;
    QLineEdit *lineCommandSeller;
    QLabel *labelCommandQuantity;
    QSpinBox *spinCommandQuantity;
    QLabel *labelCommandDate;
    QDateEdit *dateCommandOrder;
    QLabel *labelCommandDelivery;
    QDateEdit *dateCommandDelivery;
    QLabel *labelCommandStatus;
    QComboBox *comboCommandStatus;
    QLabel *labelCommandTotal;
    QDoubleSpinBox *spinCommandTotal;
    QLabel *labelCommandEmail;
    QLineEdit *lineCommandEmail;
    QHBoxLayout *horizontalLayout_commandActions;
    QPushButton *confirmCommandButton;
    QPushButton *cancelCommandButton;
    QGroupBox *groupListCommands;
    QVBoxLayout *verticalLayout_listCommands;
    QHBoxLayout *horizontalLayout_searchCommands;
    QLineEdit *searchCommands;
    QHBoxLayout *horizontalLayout_filterCommands;
    QLabel *labelFilterCommandStatus;
    QComboBox *comboFilterCommandStatus;
    QLabel *labelSortBy;
    QComboBox *comboSortBy;
    QTableWidget *tableCommands;
    QHBoxLayout *horizontalLayout_commandsActions;
    QPushButton *statisticsCommandButton;
    QPushButton *exportCommandButton;
    QPushButton *sendNotificationButton;
    QPushButton *deleteCommandButton;
    QWidget *tabStocks;
    QVBoxLayout *verticalLayout_stocks;
    QLabel *labelStocksTitle;
    QTableWidget *tableStocks;
    QWidget *tabFunctionality;
    QVBoxLayout *verticalLayout_functionality;
    QLabel *labelFunc;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1025, 704);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        sideFrame = new QFrame(centralwidget);
        sideFrame->setObjectName("sideFrame");
        sideFrame->setMinimumSize(QSize(160, 0));
        verticalLayout_side = new QVBoxLayout(sideFrame);
        verticalLayout_side->setObjectName("verticalLayout_side");
        clientsButton = new QPushButton(sideFrame);
        clientsButton->setObjectName("clientsButton");

        verticalLayout_side->addWidget(clientsButton);

        commandsButton = new QPushButton(sideFrame);
        commandsButton->setObjectName("commandsButton");

        verticalLayout_side->addWidget(commandsButton);

        stocksButton = new QPushButton(sideFrame);
        stocksButton->setObjectName("stocksButton");

        verticalLayout_side->addWidget(stocksButton);

        verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_side->addItem(verticalSpacer);

        logoutButton = new QPushButton(sideFrame);
        logoutButton->setObjectName("logoutButton");

        verticalLayout_side->addWidget(logoutButton);


        horizontalLayout->addWidget(sideFrame);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabHome = new QWidget();
        tabHome->setObjectName("tabHome");
        horizontalLayout_main = new QHBoxLayout(tabHome);
        horizontalLayout_main->setObjectName("horizontalLayout_main");
        groupAdd = new QGroupBox(tabHome);
        groupAdd->setObjectName("groupAdd");
        formLayout = new QFormLayout(groupAdd);
        formLayout->setObjectName("formLayout");
        labelSKU = new QLabel(groupAdd);
        labelSKU->setObjectName("labelSKU");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelSKU);

        lineSKU = new QLineEdit(groupAdd);
        lineSKU->setObjectName("lineSKU");

        formLayout->setWidget(0, QFormLayout::FieldRole, lineSKU);

        labelName = new QLabel(groupAdd);
        labelName->setObjectName("labelName");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelName);

        lineName = new QLineEdit(groupAdd);
        lineName->setObjectName("lineName");

        formLayout->setWidget(1, QFormLayout::FieldRole, lineName);

        labelCategory = new QLabel(groupAdd);
        labelCategory->setObjectName("labelCategory");

        formLayout->setWidget(2, QFormLayout::LabelRole, labelCategory);

        comboCategory = new QComboBox(groupAdd);
        comboCategory->setObjectName("comboCategory");

        formLayout->setWidget(2, QFormLayout::FieldRole, comboCategory);

        labelBrand = new QLabel(groupAdd);
        labelBrand->setObjectName("labelBrand");

        formLayout->setWidget(3, QFormLayout::LabelRole, labelBrand);

        comboBrand = new QComboBox(groupAdd);
        comboBrand->setObjectName("comboBrand");

        formLayout->setWidget(3, QFormLayout::FieldRole, comboBrand);

        labelPurchase = new QLabel(groupAdd);
        labelPurchase->setObjectName("labelPurchase");

        formLayout->setWidget(4, QFormLayout::LabelRole, labelPurchase);

        linePurchase = new QLineEdit(groupAdd);
        linePurchase->setObjectName("linePurchase");

        formLayout->setWidget(4, QFormLayout::FieldRole, linePurchase);

        labelSelling = new QLabel(groupAdd);
        labelSelling->setObjectName("labelSelling");

        formLayout->setWidget(5, QFormLayout::LabelRole, labelSelling);

        lineSelling = new QLineEdit(groupAdd);
        lineSelling->setObjectName("lineSelling");

        formLayout->setWidget(5, QFormLayout::FieldRole, lineSelling);

        labelStock = new QLabel(groupAdd);
        labelStock->setObjectName("labelStock");

        formLayout->setWidget(6, QFormLayout::LabelRole, labelStock);

        spinStock = new QSpinBox(groupAdd);
        spinStock->setObjectName("spinStock");

        formLayout->setWidget(6, QFormLayout::FieldRole, spinStock);

        labelLocation = new QLabel(groupAdd);
        labelLocation->setObjectName("labelLocation");

        formLayout->setWidget(7, QFormLayout::LabelRole, labelLocation);

        lineLocation = new QLineEdit(groupAdd);
        lineLocation->setObjectName("lineLocation");

        formLayout->setWidget(7, QFormLayout::FieldRole, lineLocation);

        horizontalLayout_actions = new QHBoxLayout();
        horizontalLayout_actions->setObjectName("horizontalLayout_actions");
        confirmButton = new QPushButton(groupAdd);
        confirmButton->setObjectName("confirmButton");

        horizontalLayout_actions->addWidget(confirmButton);

        cancelButton = new QPushButton(groupAdd);
        cancelButton->setObjectName("cancelButton");

        horizontalLayout_actions->addWidget(cancelButton);


        formLayout->setLayout(8, QFormLayout::SpanningRole, horizontalLayout_actions);


        horizontalLayout_main->addWidget(groupAdd);

        groupList = new QGroupBox(tabHome);
        groupList->setObjectName("groupList");
        verticalLayout_list = new QVBoxLayout(groupList);
        verticalLayout_list->setObjectName("verticalLayout_list");
        searchLine = new QLineEdit(groupList);
        searchLine->setObjectName("searchLine");

        verticalLayout_list->addWidget(searchLine);

        tableArticles = new QTableWidget(groupList);
        if (tableArticles->columnCount() < 5)
            tableArticles->setColumnCount(5);
        tableArticles->setObjectName("tableArticles");
        tableArticles->setColumnCount(5);

        verticalLayout_list->addWidget(tableArticles);

        horizontalLayout_tableActions = new QHBoxLayout();
        horizontalLayout_tableActions->setObjectName("horizontalLayout_tableActions");
        exportButton = new QPushButton(groupList);
        exportButton->setObjectName("exportButton");

        horizontalLayout_tableActions->addWidget(exportButton);

        deleteButton = new QPushButton(groupList);
        deleteButton->setObjectName("deleteButton");

        horizontalLayout_tableActions->addWidget(deleteButton);


        verticalLayout_list->addLayout(horizontalLayout_tableActions);


        horizontalLayout_main->addWidget(groupList);

        tabWidget->addTab(tabHome, QString());
        tabMatieres = new QWidget();
        tabMatieres->setObjectName("tabMatieres");
        horizontalLayout_matieres = new QHBoxLayout(tabMatieres);
        horizontalLayout_matieres->setObjectName("horizontalLayout_matieres");
        groupAddMatiere = new QGroupBox(tabMatieres);
        groupAddMatiere->setObjectName("groupAddMatiere");
        formLayout_matiere = new QFormLayout(groupAddMatiere);
        formLayout_matiere->setObjectName("formLayout_matiere");
        labelMatiereName = new QLabel(groupAddMatiere);
        labelMatiereName->setObjectName("labelMatiereName");

        formLayout_matiere->setWidget(0, QFormLayout::LabelRole, labelMatiereName);

        lineMatiereName = new QLineEdit(groupAddMatiere);
        lineMatiereName->setObjectName("lineMatiereName");

        formLayout_matiere->setWidget(0, QFormLayout::FieldRole, lineMatiereName);

        labelMatiereType = new QLabel(groupAddMatiere);
        labelMatiereType->setObjectName("labelMatiereType");

        formLayout_matiere->setWidget(1, QFormLayout::LabelRole, labelMatiereType);

        comboMatiereType = new QComboBox(groupAddMatiere);
        comboMatiereType->addItem(QString());
        comboMatiereType->addItem(QString());
        comboMatiereType->addItem(QString());
        comboMatiereType->addItem(QString());
        comboMatiereType->addItem(QString());
        comboMatiereType->addItem(QString());
        comboMatiereType->addItem(QString());
        comboMatiereType->addItem(QString());
        comboMatiereType->addItem(QString());
        comboMatiereType->setObjectName("comboMatiereType");

        formLayout_matiere->setWidget(1, QFormLayout::FieldRole, comboMatiereType);

        labelMatiereQuantite = new QLabel(groupAddMatiere);
        labelMatiereQuantite->setObjectName("labelMatiereQuantite");

        formLayout_matiere->setWidget(2, QFormLayout::LabelRole, labelMatiereQuantite);

        spinMatiereQuantite = new QSpinBox(groupAddMatiere);
        spinMatiereQuantite->setObjectName("spinMatiereQuantite");

        formLayout_matiere->setWidget(2, QFormLayout::FieldRole, spinMatiereQuantite);

        labelMatiereCouleur = new QLabel(groupAddMatiere);
        labelMatiereCouleur->setObjectName("labelMatiereCouleur");

        formLayout_matiere->setWidget(3, QFormLayout::LabelRole, labelMatiereCouleur);

        lineMatiereColor = new QLineEdit(groupAddMatiere);
        lineMatiereColor->setObjectName("lineMatiereColor");

        formLayout_matiere->setWidget(3, QFormLayout::FieldRole, lineMatiereColor);

        labelMatierePerte = new QLabel(groupAddMatiere);
        labelMatierePerte->setObjectName("labelMatierePerte");

        formLayout_matiere->setWidget(4, QFormLayout::LabelRole, labelMatierePerte);

        lineMatiereWaste = new QLineEdit(groupAddMatiere);
        lineMatiereWaste->setObjectName("lineMatiereWaste");

        formLayout_matiere->setWidget(4, QFormLayout::FieldRole, lineMatiereWaste);

        labelMatiereStatut = new QLabel(groupAddMatiere);
        labelMatiereStatut->setObjectName("labelMatiereStatut");

        formLayout_matiere->setWidget(5, QFormLayout::LabelRole, labelMatiereStatut);

        comboMatiereStatus = new QComboBox(groupAddMatiere);
        comboMatiereStatus->addItem(QString());
        comboMatiereStatus->addItem(QString());
        comboMatiereStatus->addItem(QString());
        comboMatiereStatus->addItem(QString());
        comboMatiereStatus->setObjectName("comboMatiereStatus");

        formLayout_matiere->setWidget(5, QFormLayout::FieldRole, comboMatiereStatus);

        horizontalLayout_matiereActions = new QHBoxLayout();
        horizontalLayout_matiereActions->setObjectName("horizontalLayout_matiereActions");
        confirmMatierButton = new QPushButton(groupAddMatiere);
        confirmMatierButton->setObjectName("confirmMatierButton");

        horizontalLayout_matiereActions->addWidget(confirmMatierButton);

        cancelMatierButton = new QPushButton(groupAddMatiere);
        cancelMatierButton->setObjectName("cancelMatierButton");

        horizontalLayout_matiereActions->addWidget(cancelMatierButton);


        formLayout_matiere->setLayout(7, QFormLayout::SpanningRole, horizontalLayout_matiereActions);

        timeEdit = new QTimeEdit(groupAddMatiere);
        timeEdit->setObjectName("timeEdit");

        formLayout_matiere->setWidget(6, QFormLayout::LabelRole, timeEdit);


        horizontalLayout_matieres->addWidget(groupAddMatiere);

        groupListMatieres = new QGroupBox(tabMatieres);
        groupListMatieres->setObjectName("groupListMatieres");
        verticalLayout_listMatieres = new QVBoxLayout(groupListMatieres);
        verticalLayout_listMatieres->setObjectName("verticalLayout_listMatieres");
        searchMatieres = new QLineEdit(groupListMatieres);
        searchMatieres->setObjectName("searchMatieres");

        verticalLayout_listMatieres->addWidget(searchMatieres);

        horizontalLayout_filterMatieres = new QHBoxLayout();
        horizontalLayout_filterMatieres->setObjectName("horizontalLayout_filterMatieres");
        labelFilterStatus = new QLabel(groupListMatieres);
        labelFilterStatus->setObjectName("labelFilterStatus");

        horizontalLayout_filterMatieres->addWidget(labelFilterStatus);

        comboFilterStatus = new QComboBox(groupListMatieres);
        comboFilterStatus->addItem(QString());
        comboFilterStatus->addItem(QString());
        comboFilterStatus->addItem(QString());
        comboFilterStatus->addItem(QString());
        comboFilterStatus->addItem(QString());
        comboFilterStatus->setObjectName("comboFilterStatus");

        horizontalLayout_filterMatieres->addWidget(comboFilterStatus);


        verticalLayout_listMatieres->addLayout(horizontalLayout_filterMatieres);

        tableMatieres = new QTableWidget(groupListMatieres);
        if (tableMatieres->columnCount() < 6)
            tableMatieres->setColumnCount(6);
        tableMatieres->setObjectName("tableMatieres");
        tableMatieres->setColumnCount(6);

        verticalLayout_listMatieres->addWidget(tableMatieres);

        horizontalLayout_matieresActions = new QHBoxLayout();
        horizontalLayout_matieresActions->setObjectName("horizontalLayout_matieresActions");
        statisticsButton = new QPushButton(groupListMatieres);
        statisticsButton->setObjectName("statisticsButton");

        horizontalLayout_matieresActions->addWidget(statisticsButton);

        exportMatiereButton = new QPushButton(groupListMatieres);
        exportMatiereButton->setObjectName("exportMatiereButton");

        horizontalLayout_matieresActions->addWidget(exportMatiereButton);

        deleteMatierButton = new QPushButton(groupListMatieres);
        deleteMatierButton->setObjectName("deleteMatierButton");

        horizontalLayout_matieresActions->addWidget(deleteMatierButton);


        verticalLayout_listMatieres->addLayout(horizontalLayout_matieresActions);


        horizontalLayout_matieres->addWidget(groupListMatieres);

        tabWidget->addTab(tabMatieres, QString());
        tabCommands = new QWidget();
        tabCommands->setObjectName("tabCommands");
        horizontalLayout_commands = new QHBoxLayout(tabCommands);
        horizontalLayout_commands->setObjectName("horizontalLayout_commands");
        groupAddCommand = new QGroupBox(tabCommands);
        groupAddCommand->setObjectName("groupAddCommand");
        formLayout_command = new QFormLayout(groupAddCommand);
        formLayout_command->setObjectName("formLayout_command");
        labelCommandID = new QLabel(groupAddCommand);
        labelCommandID->setObjectName("labelCommandID");

        formLayout_command->setWidget(0, QFormLayout::LabelRole, labelCommandID);

        lineCommandID = new QLineEdit(groupAddCommand);
        lineCommandID->setObjectName("lineCommandID");

        formLayout_command->setWidget(0, QFormLayout::FieldRole, lineCommandID);

        labelCommandType = new QLabel(groupAddCommand);
        labelCommandType->setObjectName("labelCommandType");

        formLayout_command->setWidget(1, QFormLayout::LabelRole, labelCommandType);

        comboCommandType = new QComboBox(groupAddCommand);
        comboCommandType->addItem(QString());
        comboCommandType->addItem(QString());
        comboCommandType->addItem(QString());
        comboCommandType->addItem(QString());
        comboCommandType->addItem(QString());
        comboCommandType->setObjectName("comboCommandType");

        formLayout_command->setWidget(1, QFormLayout::FieldRole, comboCommandType);

        labelCommandSeller = new QLabel(groupAddCommand);
        labelCommandSeller->setObjectName("labelCommandSeller");

        formLayout_command->setWidget(2, QFormLayout::LabelRole, labelCommandSeller);

        lineCommandSeller = new QLineEdit(groupAddCommand);
        lineCommandSeller->setObjectName("lineCommandSeller");

        formLayout_command->setWidget(2, QFormLayout::FieldRole, lineCommandSeller);

        labelCommandQuantity = new QLabel(groupAddCommand);
        labelCommandQuantity->setObjectName("labelCommandQuantity");

        formLayout_command->setWidget(3, QFormLayout::LabelRole, labelCommandQuantity);

        spinCommandQuantity = new QSpinBox(groupAddCommand);
        spinCommandQuantity->setObjectName("spinCommandQuantity");

        formLayout_command->setWidget(3, QFormLayout::FieldRole, spinCommandQuantity);

        labelCommandDate = new QLabel(groupAddCommand);
        labelCommandDate->setObjectName("labelCommandDate");

        formLayout_command->setWidget(4, QFormLayout::LabelRole, labelCommandDate);

        dateCommandOrder = new QDateEdit(groupAddCommand);
        dateCommandOrder->setObjectName("dateCommandOrder");

        formLayout_command->setWidget(4, QFormLayout::FieldRole, dateCommandOrder);

        labelCommandDelivery = new QLabel(groupAddCommand);
        labelCommandDelivery->setObjectName("labelCommandDelivery");

        formLayout_command->setWidget(5, QFormLayout::LabelRole, labelCommandDelivery);

        dateCommandDelivery = new QDateEdit(groupAddCommand);
        dateCommandDelivery->setObjectName("dateCommandDelivery");

        formLayout_command->setWidget(5, QFormLayout::FieldRole, dateCommandDelivery);

        labelCommandStatus = new QLabel(groupAddCommand);
        labelCommandStatus->setObjectName("labelCommandStatus");

        formLayout_command->setWidget(6, QFormLayout::LabelRole, labelCommandStatus);

        comboCommandStatus = new QComboBox(groupAddCommand);
        comboCommandStatus->addItem(QString());
        comboCommandStatus->addItem(QString());
        comboCommandStatus->addItem(QString());
        comboCommandStatus->addItem(QString());
        comboCommandStatus->addItem(QString());
        comboCommandStatus->addItem(QString());
        comboCommandStatus->setObjectName("comboCommandStatus");

        formLayout_command->setWidget(6, QFormLayout::FieldRole, comboCommandStatus);

        labelCommandTotal = new QLabel(groupAddCommand);
        labelCommandTotal->setObjectName("labelCommandTotal");

        formLayout_command->setWidget(7, QFormLayout::LabelRole, labelCommandTotal);

        spinCommandTotal = new QDoubleSpinBox(groupAddCommand);
        spinCommandTotal->setObjectName("spinCommandTotal");
        spinCommandTotal->setDecimals(2);
        spinCommandTotal->setMinimum(0.000000000000000);
        spinCommandTotal->setMaximum(999999.989999999990687);
        spinCommandTotal->setSingleStep(0.010000000000000);
        spinCommandTotal->setValue(0.000000000000000);

        formLayout_command->setWidget(7, QFormLayout::FieldRole, spinCommandTotal);

        labelCommandEmail = new QLabel(groupAddCommand);
        labelCommandEmail->setObjectName("labelCommandEmail");

        formLayout_command->setWidget(8, QFormLayout::LabelRole, labelCommandEmail);

        lineCommandEmail = new QLineEdit(groupAddCommand);
        lineCommandEmail->setObjectName("lineCommandEmail");

        formLayout_command->setWidget(8, QFormLayout::FieldRole, lineCommandEmail);

        horizontalLayout_commandActions = new QHBoxLayout();
        horizontalLayout_commandActions->setObjectName("horizontalLayout_commandActions");
        confirmCommandButton = new QPushButton(groupAddCommand);
        confirmCommandButton->setObjectName("confirmCommandButton");

        horizontalLayout_commandActions->addWidget(confirmCommandButton);

        cancelCommandButton = new QPushButton(groupAddCommand);
        cancelCommandButton->setObjectName("cancelCommandButton");

        horizontalLayout_commandActions->addWidget(cancelCommandButton);


        formLayout_command->setLayout(9, QFormLayout::SpanningRole, horizontalLayout_commandActions);


        horizontalLayout_commands->addWidget(groupAddCommand);

        groupListCommands = new QGroupBox(tabCommands);
        groupListCommands->setObjectName("groupListCommands");
        verticalLayout_listCommands = new QVBoxLayout(groupListCommands);
        verticalLayout_listCommands->setObjectName("verticalLayout_listCommands");
        horizontalLayout_searchCommands = new QHBoxLayout();
        horizontalLayout_searchCommands->setObjectName("horizontalLayout_searchCommands");
        searchCommands = new QLineEdit(groupListCommands);
        searchCommands->setObjectName("searchCommands");

        horizontalLayout_searchCommands->addWidget(searchCommands);


        verticalLayout_listCommands->addLayout(horizontalLayout_searchCommands);

        horizontalLayout_filterCommands = new QHBoxLayout();
        horizontalLayout_filterCommands->setObjectName("horizontalLayout_filterCommands");
        labelFilterCommandStatus = new QLabel(groupListCommands);
        labelFilterCommandStatus->setObjectName("labelFilterCommandStatus");

        horizontalLayout_filterCommands->addWidget(labelFilterCommandStatus);

        comboFilterCommandStatus = new QComboBox(groupListCommands);
        comboFilterCommandStatus->addItem(QString());
        comboFilterCommandStatus->addItem(QString());
        comboFilterCommandStatus->addItem(QString());
        comboFilterCommandStatus->addItem(QString());
        comboFilterCommandStatus->addItem(QString());
        comboFilterCommandStatus->addItem(QString());
        comboFilterCommandStatus->setObjectName("comboFilterCommandStatus");

        horizontalLayout_filterCommands->addWidget(comboFilterCommandStatus);

        labelSortBy = new QLabel(groupListCommands);
        labelSortBy->setObjectName("labelSortBy");

        horizontalLayout_filterCommands->addWidget(labelSortBy);

        comboSortBy = new QComboBox(groupListCommands);
        comboSortBy->addItem(QString());
        comboSortBy->addItem(QString());
        comboSortBy->addItem(QString());
        comboSortBy->setObjectName("comboSortBy");

        horizontalLayout_filterCommands->addWidget(comboSortBy);


        verticalLayout_listCommands->addLayout(horizontalLayout_filterCommands);

        tableCommands = new QTableWidget(groupListCommands);
        if (tableCommands->columnCount() < 9)
            tableCommands->setColumnCount(9);
        tableCommands->setObjectName("tableCommands");
        tableCommands->setColumnCount(9);

        verticalLayout_listCommands->addWidget(tableCommands);

        horizontalLayout_commandsActions = new QHBoxLayout();
        horizontalLayout_commandsActions->setObjectName("horizontalLayout_commandsActions");
        statisticsCommandButton = new QPushButton(groupListCommands);
        statisticsCommandButton->setObjectName("statisticsCommandButton");

        horizontalLayout_commandsActions->addWidget(statisticsCommandButton);

        exportCommandButton = new QPushButton(groupListCommands);
        exportCommandButton->setObjectName("exportCommandButton");

        horizontalLayout_commandsActions->addWidget(exportCommandButton);

        sendNotificationButton = new QPushButton(groupListCommands);
        sendNotificationButton->setObjectName("sendNotificationButton");

        horizontalLayout_commandsActions->addWidget(sendNotificationButton);

        deleteCommandButton = new QPushButton(groupListCommands);
        deleteCommandButton->setObjectName("deleteCommandButton");

        horizontalLayout_commandsActions->addWidget(deleteCommandButton);


        verticalLayout_listCommands->addLayout(horizontalLayout_commandsActions);


        horizontalLayout_commands->addWidget(groupListCommands);

        tabWidget->addTab(tabCommands, QString());
        tabStocks = new QWidget();
        tabStocks->setObjectName("tabStocks");
        verticalLayout_stocks = new QVBoxLayout(tabStocks);
        verticalLayout_stocks->setObjectName("verticalLayout_stocks");
        labelStocksTitle = new QLabel(tabStocks);
        labelStocksTitle->setObjectName("labelStocksTitle");

        verticalLayout_stocks->addWidget(labelStocksTitle);

        tableStocks = new QTableWidget(tabStocks);
        if (tableStocks->columnCount() < 4)
            tableStocks->setColumnCount(4);
        tableStocks->setObjectName("tableStocks");
        tableStocks->setColumnCount(4);

        verticalLayout_stocks->addWidget(tableStocks);

        tabWidget->addTab(tabStocks, QString());
        tabFunctionality = new QWidget();
        tabFunctionality->setObjectName("tabFunctionality");
        verticalLayout_functionality = new QVBoxLayout(tabFunctionality);
        verticalLayout_functionality->setObjectName("verticalLayout_functionality");
        labelFunc = new QLabel(tabFunctionality);
        labelFunc->setObjectName("labelFunc");

        verticalLayout_functionality->addWidget(labelFunc);

        tabWidget->addTab(tabFunctionality, QString());

        horizontalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1025, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Inventory", nullptr));
        MainWindow->setStyleSheet(QCoreApplication::translate("MainWindow", "QMainWindow {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #0d0d0d, stop:1 #1a1a1a);\n"
"}\n"
"QWidget {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #0d0d0d, stop:1 #1a1a1a);\n"
"    color: #f5f5dc;\n"
"}\n"
"QPushButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #A0522D, stop:1 #8B4513);\n"
"    color: white;\n"
"    border: 2px solid #6B3410;\n"
"    padding: 10px 15px;\n"
"    border-radius: 8px;\n"
"    font-weight: bold;\n"
"    font-size: 11px;\n"
"    outline: none;\n"
"}\n"
"QPushButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B8613C, stop:1 #9B5024);\n"
"    border: 2px solid #8B4513;\n"
"}\n"
"QPushButton:pressed {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #6B3410, stop:1 #5a2a0b);\n"
"    border: 2px solid #4a1f08;\n"
"}\n"
"QLineEdit, QSpinBox, QComboBox, QDateEdit, QDoubleSpinBox {\n"
"    background-color: #f5f5dc;\n"
"    color: #1a1a1a;\n"
"    border: 2px solid #8B"
                        "7355;\n"
"    padding: 8px;\n"
"    border-radius: 6px;\n"
"    font-weight: 500;\n"
"}\n"
"QLineEdit:focus, QSpinBox:focus, QComboBox:focus, QDateEdit:focus, QDoubleSpinBox:focus {\n"
"    border: 2px solid #A0522D;\n"
"    background-color: #fffef9;\n"
"}\n"
"QLabel {\n"
"    color: #f5f5dc;\n"
"    font-weight: bold;\n"
"    font-size: 10pt;\n"
"}\n"
"QGroupBox {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #8B6F47, stop:1 #7A5F38);\n"
"    color: #ffffff;\n"
"    border: 3px solid #A0522D;\n"
"    border-radius: 10px;\n"
"    padding: 15px;\n"
"    font-weight: bold;\n"
"    font-size: 11pt;\n"
"    margin-top: 10px;\n"
"}\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 15px;\n"
"    padding: 0 5px 0 5px;\n"
"    color: #f5f5dc;\n"
"    font-weight: bold;\n"
"}\n"
"QTabWidget::pane {\n"
"    border: 3px solid #A0522D;\n"
"    background-color: #1a1a1a;\n"
"}\n"
"QTabBar::tab {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #6B5D52, stop:1 #5"
                        "44945);\n"
"    color: #d4af99;\n"
"    padding: 10px 25px;\n"
"    border: 2px solid #4a423c;\n"
"    border-radius: 6px 6px 0px 0px;\n"
"    margin-right: 2px;\n"
"    font-weight: bold;\n"
"}\n"
"QTabBar::tab:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #7a6b5f, stop:1 #5a5248);\n"
"}\n"
"QTabBar::tab:selected {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B8613C, stop:1 #A0522D);\n"
"    color: #ffffff;\n"
"    border: 2px solid #8B4513;\n"
"}\n"
"QTableWidget {\n"
"    background-color: #D2B48C;\n"
"    alternate-background-color: #E8D5C4;\n"
"    gridline-color: #A0826D;\n"
"    border: 2px solid #8B7355;\n"
"    border-radius: 6px;\n"
"}\n"
"QTableWidget::item {\n"
"    padding: 8px;\n"
"    border-right: 1px solid #A0826D;\n"
"    border-bottom: 1px solid #A0826D;\n"
"}\n"
"QTableWidget::item:selected {\n"
"    background-color: #A0522D;\n"
"    color: #ffffff;\n"
"}\n"
"QHeaderView::section {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:"
                        "1, stop:0 #A0522D, stop:1 #8B4513);\n"
"    color: #ffffff;\n"
"    padding: 8px;\n"
"    border: none;\n"
"    border-right: 1px solid #6B3410;\n"
"    font-weight: bold;\n"
"    font-size: 10pt;\n"
"}\n"
"QScrollBar:vertical {\n"
"    background-color: #2a2a2a;\n"
"    width: 12px;\n"
"    border: 1px solid #4a423c;\n"
"}\n"
"QScrollBar::handle:vertical {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #8B7355, stop:1 #A0826D);\n"
"    border-radius: 6px;\n"
"    min-height: 20px;\n"
"}\n"
"QScrollBar::handle:vertical:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #A0826D, stop:1 #B8967D);\n"
"}", nullptr));
        clientsButton->setText(QCoreApplication::translate("MainWindow", "MATI\303\210RES", nullptr));
        commandsButton->setText(QCoreApplication::translate("MainWindow", "COMMANDS", nullptr));
        stocksButton->setText(QCoreApplication::translate("MainWindow", "EMPLOY\303\211", nullptr));
        logoutButton->setText(QCoreApplication::translate("MainWindow", "LOG OUT", nullptr));
        groupAdd->setTitle(QCoreApplication::translate("MainWindow", "Add an Article", nullptr));
        labelSKU->setText(QCoreApplication::translate("MainWindow", "SKU :", nullptr));
        labelName->setText(QCoreApplication::translate("MainWindow", "Name :", nullptr));
        labelCategory->setText(QCoreApplication::translate("MainWindow", "Category :", nullptr));
        labelBrand->setText(QCoreApplication::translate("MainWindow", "Brand :", nullptr));
        labelPurchase->setText(QCoreApplication::translate("MainWindow", "Purchase price :", nullptr));
        labelSelling->setText(QCoreApplication::translate("MainWindow", "Selling Price :", nullptr));
        labelStock->setText(QCoreApplication::translate("MainWindow", "Stock :", nullptr));
        labelLocation->setText(QCoreApplication::translate("MainWindow", "Location :", nullptr));
        confirmButton->setText(QCoreApplication::translate("MainWindow", "Confirm", nullptr));
        cancelButton->setText(QCoreApplication::translate("MainWindow", "Cancel", nullptr));
        groupList->setTitle(QCoreApplication::translate("MainWindow", "Articles", nullptr));
        searchLine->setPlaceholderText(QCoreApplication::translate("MainWindow", "Search for an Article...", nullptr));
        exportButton->setText(QCoreApplication::translate("MainWindow", "Export PDF", nullptr));
        deleteButton->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabHome), QCoreApplication::translate("MainWindow", "Home", nullptr));
        groupAddMatiere->setTitle(QCoreApplication::translate("MainWindow", "Add a Raw Material", nullptr));
        labelMatiereName->setText(QCoreApplication::translate("MainWindow", "Name :", nullptr));
        labelMatiereType->setText(QCoreApplication::translate("MainWindow", "Type :", nullptr));
        comboMatiereType->setItemText(0, QCoreApplication::translate("MainWindow", "Caramel", nullptr));
        comboMatiereType->setItemText(1, QCoreApplication::translate("MainWindow", "Wine Red", nullptr));
        comboMatiereType->setItemText(2, QCoreApplication::translate("MainWindow", "Light Brown", nullptr));
        comboMatiereType->setItemText(3, QCoreApplication::translate("MainWindow", "Mahogany", nullptr));
        comboMatiereType->setItemText(4, QCoreApplication::translate("MainWindow", "Black", nullptr));
        comboMatiereType->setItemText(5, QCoreApplication::translate("MainWindow", "Dark Brown", nullptr));
        comboMatiereType->setItemText(6, QCoreApplication::translate("MainWindow", "Saddle Brown", nullptr));
        comboMatiereType->setItemText(7, QCoreApplication::translate("MainWindow", "Rugged Brown", nullptr));
        comboMatiereType->setItemText(8, QCoreApplication::translate("MainWindow", "Natural", nullptr));

        labelMatiereQuantite->setText(QCoreApplication::translate("MainWindow", "Quantity :", nullptr));
        labelMatiereCouleur->setText(QCoreApplication::translate("MainWindow", "Color/Yield :", nullptr));
        labelMatierePerte->setText(QCoreApplication::translate("MainWindow", "Loss :", nullptr));
        labelMatiereStatut->setText(QCoreApplication::translate("MainWindow", "Status :", nullptr));
        comboMatiereStatus->setItemText(0, QCoreApplication::translate("MainWindow", "In Stock", nullptr));
        comboMatiereStatus->setItemText(1, QCoreApplication::translate("MainWindow", "In Production", nullptr));
        comboMatiereStatus->setItemText(2, QCoreApplication::translate("MainWindow", "Low Stock", nullptr));
        comboMatiereStatus->setItemText(3, QCoreApplication::translate("MainWindow", "Out of Stock", nullptr));

        confirmMatierButton->setText(QCoreApplication::translate("MainWindow", "Confirm", nullptr));
        cancelMatierButton->setText(QCoreApplication::translate("MainWindow", "Cancel", nullptr));
        groupListMatieres->setTitle(QCoreApplication::translate("MainWindow", "Raw Materials List", nullptr));
        searchMatieres->setPlaceholderText(QCoreApplication::translate("MainWindow", "Search for a material...", nullptr));
        labelFilterStatus->setText(QCoreApplication::translate("MainWindow", "Filter by Status :", nullptr));
        comboFilterStatus->setItemText(0, QCoreApplication::translate("MainWindow", "All", nullptr));
        comboFilterStatus->setItemText(1, QCoreApplication::translate("MainWindow", "In Stock", nullptr));
        comboFilterStatus->setItemText(2, QCoreApplication::translate("MainWindow", "In Production", nullptr));
        comboFilterStatus->setItemText(3, QCoreApplication::translate("MainWindow", "Low Stock", nullptr));
        comboFilterStatus->setItemText(4, QCoreApplication::translate("MainWindow", "Out of Stock", nullptr));

        statisticsButton->setText(QCoreApplication::translate("MainWindow", "Statistics", nullptr));
        exportMatiereButton->setText(QCoreApplication::translate("MainWindow", "Export PDF", nullptr));
        deleteMatierButton->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabMatieres), QCoreApplication::translate("MainWindow", "Mati\303\250res", nullptr));
        groupAddCommand->setTitle(QCoreApplication::translate("MainWindow", "Add a Command", nullptr));
        labelCommandID->setText(QCoreApplication::translate("MainWindow", "Command ID :", nullptr));
        labelCommandType->setText(QCoreApplication::translate("MainWindow", "Type :", nullptr));
        comboCommandType->setItemText(0, QCoreApplication::translate("MainWindow", "Veste", nullptr));
        comboCommandType->setItemText(1, QCoreApplication::translate("MainWindow", "Sac", nullptr));
        comboCommandType->setItemText(2, QCoreApplication::translate("MainWindow", "Chaussure", nullptr));
        comboCommandType->setItemText(3, QCoreApplication::translate("MainWindow", "Ceinture", nullptr));
        comboCommandType->setItemText(4, QCoreApplication::translate("MainWindow", "Autre", nullptr));

        labelCommandSeller->setText(QCoreApplication::translate("MainWindow", "Responsible (Seller) :", nullptr));
        labelCommandQuantity->setText(QCoreApplication::translate("MainWindow", "Quantity :", nullptr));
        labelCommandDate->setText(QCoreApplication::translate("MainWindow", "Order Date :", nullptr));
        labelCommandDelivery->setText(QCoreApplication::translate("MainWindow", "Delivery Date :", nullptr));
        labelCommandStatus->setText(QCoreApplication::translate("MainWindow", "Status :", nullptr));
        comboCommandStatus->setItemText(0, QCoreApplication::translate("MainWindow", "Pending", nullptr));
        comboCommandStatus->setItemText(1, QCoreApplication::translate("MainWindow", "In Production", nullptr));
        comboCommandStatus->setItemText(2, QCoreApplication::translate("MainWindow", "Ready", nullptr));
        comboCommandStatus->setItemText(3, QCoreApplication::translate("MainWindow", "Shipped", nullptr));
        comboCommandStatus->setItemText(4, QCoreApplication::translate("MainWindow", "Delivered", nullptr));
        comboCommandStatus->setItemText(5, QCoreApplication::translate("MainWindow", "Cancelled", nullptr));

        labelCommandTotal->setText(QCoreApplication::translate("MainWindow", "Total Price :", nullptr));
        labelCommandEmail->setText(QCoreApplication::translate("MainWindow", "Email :", nullptr));
        confirmCommandButton->setText(QCoreApplication::translate("MainWindow", "Confirm", nullptr));
        cancelCommandButton->setText(QCoreApplication::translate("MainWindow", "Cancel", nullptr));
        groupListCommands->setTitle(QCoreApplication::translate("MainWindow", "Commands List", nullptr));
        searchCommands->setPlaceholderText(QCoreApplication::translate("MainWindow", "Search by ID, Type, Seller...", nullptr));
        labelFilterCommandStatus->setText(QCoreApplication::translate("MainWindow", "Status :", nullptr));
        comboFilterCommandStatus->setItemText(0, QCoreApplication::translate("MainWindow", "All", nullptr));
        comboFilterCommandStatus->setItemText(1, QCoreApplication::translate("MainWindow", "Pending", nullptr));
        comboFilterCommandStatus->setItemText(2, QCoreApplication::translate("MainWindow", "In Production", nullptr));
        comboFilterCommandStatus->setItemText(3, QCoreApplication::translate("MainWindow", "Ready", nullptr));
        comboFilterCommandStatus->setItemText(4, QCoreApplication::translate("MainWindow", "Shipped", nullptr));
        comboFilterCommandStatus->setItemText(5, QCoreApplication::translate("MainWindow", "Delivered", nullptr));

        labelSortBy->setText(QCoreApplication::translate("MainWindow", "Sort By :", nullptr));
        comboSortBy->setItemText(0, QCoreApplication::translate("MainWindow", "Date", nullptr));
        comboSortBy->setItemText(1, QCoreApplication::translate("MainWindow", "Quantity", nullptr));
        comboSortBy->setItemText(2, QCoreApplication::translate("MainWindow", "Status", nullptr));

        statisticsCommandButton->setText(QCoreApplication::translate("MainWindow", "Statistics", nullptr));
        exportCommandButton->setText(QCoreApplication::translate("MainWindow", "Export PDF", nullptr));
        sendNotificationButton->setText(QCoreApplication::translate("MainWindow", "Send Notification", nullptr));
        deleteCommandButton->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabCommands), QCoreApplication::translate("MainWindow", "Commands", nullptr));
        labelStocksTitle->setText(QCoreApplication::translate("MainWindow", "Stocks Page", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabStocks), QCoreApplication::translate("MainWindow", "Employ\303\251", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabFunctionality), QCoreApplication::translate("MainWindow", "Functionality", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
