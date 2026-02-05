#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // Apply modern theme styling with gradients
    QString buttonStyle = "QPushButton { "
                         "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #A0522D, stop:1 #8B4513); "
                         "color: white; border: 2px solid #6B3410; padding: 10px 15px; "
                         "border-radius: 8px; font-weight: bold; font-size: 11px; } "
                         "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B8613C, stop:1 #9B5024); border: 2px solid #8B4513; } "
                         "QPushButton:pressed { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #6B3410, stop:1 #5a2a0b); border: 2px solid #4a1f08; }";
    
    ui->clientsButton->setStyleSheet(buttonStyle);
    ui->commandsButton->setStyleSheet(buttonStyle);
    ui->stocksButton->setStyleSheet(buttonStyle);
    
    QString logoutStyle = "QPushButton { "
                         "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #A0522D, stop:1 #8B4513); "
                         "color: white; border: 2px solid #6B3410; padding: 10px 15px; "
                         "border-radius: 8px; font-weight: bold; font-size: 11px; } "
                         "QPushButton:hover { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B8613C, stop:1 #9B5024); border: 2px solid #8B4513; } "
                         "QPushButton:pressed { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #6B3410, stop:1 #5a2a0b); border: 2px solid #4a1f08; }";
    
    ui->logoutButton->setStyleSheet(logoutStyle);
    
    // Apply modern sidebar styling
    ui->sideFrame->setStyleSheet("QFrame { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #0d0d0d, stop:1 #1a1a1a); border: 2px solid #A0522D; }");
    
    // Initialize Materials Tab
    initializeMaterialsTab();
    
    // Initialize Commands Tab
    initializeCommandsTab();
    
    // Connect sidebar buttons to their respective tabs
    connect(ui->clientsButton, &QPushButton::clicked, this, [this]() {
        ui->tabWidget->setCurrentWidget(ui->tabMatieres);
    });
    
    connect(ui->commandsButton, &QPushButton::clicked, this, [this]() {
        ui->tabWidget->setCurrentWidget(ui->tabCommands);
    });
    
    connect(ui->stocksButton, &QPushButton::clicked, this, [this]() {
        ui->tabWidget->setCurrentWidget(ui->tabStocks);
    });
}

void MainWindow::initializeMaterialsTab()
{
    // Setup Material Type combo box - Leather Types (Leader)
    ui->comboMatiereType->addItem("Caramel");
    ui->comboMatiereType->addItem("Wine Red");
    ui->comboMatiereType->addItem("Light Brown");
    ui->comboMatiereType->addItem("Mahogany");
    ui->comboMatiereType->addItem("Black");
    ui->comboMatiereType->addItem("Dark Brown");
    ui->comboMatiereType->addItem("Saddle Brown");
    ui->comboMatiereType->addItem("Rugged Brown");
    ui->comboMatiereType->addItem("Natural");
    
    // Setup Materials Table
    ui->tableMatieres->setColumnCount(6);
    QStringList headers;
    headers << "Name" << "Type" << "Quantity" << "Color/Yield" << "Loss" << "Status";
    ui->tableMatieres->setHorizontalHeaderLabels(headers);
    ui->tableMatieres->setColumnWidth(0, 100);
    ui->tableMatieres->setColumnWidth(1, 80);
    ui->tableMatieres->setColumnWidth(2, 80);
    ui->tableMatieres->setColumnWidth(3, 100);
    ui->tableMatieres->setColumnWidth(4, 80);
    ui->tableMatieres->setColumnWidth(5, 100);
    
    // Connect buttons
    connect(ui->confirmMatierButton, &QPushButton::clicked, this, &MainWindow::addMaterial);
    connect(ui->cancelMatierButton, &QPushButton::clicked, this, &MainWindow::clearMaterialForm);
    connect(ui->deleteMatierButton, &QPushButton::clicked, this, &MainWindow::deleteMaterial);
    connect(ui->exportMatiereButton, &QPushButton::clicked, this, &MainWindow::exportMaterials);
    connect(ui->statisticsButton, &QPushButton::clicked, this, &MainWindow::showStatistics);
    connect(ui->comboFilterStatus, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::filterMaterials);
    connect(ui->searchMatieres, &QLineEdit::textChanged, this, &MainWindow::searchMaterials);
}

void MainWindow::addMaterial()
{
    QString name = ui->lineMatiereName->text();
    QString type = ui->comboMatiereType->currentText();
    int quantity = ui->spinMatiereQuantite->value();
    QString color = ui->lineMatiereColor->text();
    QString waste = ui->lineMatiereWaste->text();
    QString status = ui->comboMatiereStatus->currentText();
    
    if (name.isEmpty() || color.isEmpty() || waste.isEmpty()) {
        qWarning() << "Please fill all fields";
        return;
    }
    
    int row = ui->tableMatieres->rowCount();
    ui->tableMatieres->insertRow(row);
    
    ui->tableMatieres->setItem(row, 0, new QTableWidgetItem(name));
    ui->tableMatieres->setItem(row, 1, new QTableWidgetItem(type));
    ui->tableMatieres->setItem(row, 2, new QTableWidgetItem(QString::number(quantity)));
    ui->tableMatieres->setItem(row, 3, new QTableWidgetItem(color));
    ui->tableMatieres->setItem(row, 4, new QTableWidgetItem(waste));
    ui->tableMatieres->setItem(row, 5, new QTableWidgetItem(status));
    
    // Update command type combo with material types
    if (!type.isEmpty() && ui->comboCommandType->findText(type) == -1) {
        ui->comboCommandType->addItem(type);
    }
    
    clearMaterialForm();
}

void MainWindow::clearMaterialForm()
{
    ui->lineMatiereName->clear();
    ui->comboMatiereType->setCurrentIndex(0);
    ui->spinMatiereQuantite->setValue(0);
    ui->lineMatiereColor->clear();
    ui->lineMatiereWaste->clear();
    ui->comboMatiereStatus->setCurrentIndex(0);
}

void MainWindow::deleteMaterial()
{
    int currentRow = ui->tableMatieres->currentRow();
    if (currentRow >= 0) {
        ui->tableMatieres->removeRow(currentRow);
    } else {
        qWarning() << "Please select a material to delete";
    }
}

void MainWindow::exportMaterials()
{
    qDebug() << "Exporting materials to PDF...";
    // TODO: Implement PDF export functionality
}

void MainWindow::showStatistics()
{
    qDebug() << "Showing material statistics...";
    // TODO: Implement statistics display
}

void MainWindow::filterMaterials()
{
    QString selectedStatus = ui->comboFilterStatus->currentText();
    
    for (int row = 0; row < ui->tableMatieres->rowCount(); ++row) {
        QTableWidgetItem* statusItem = ui->tableMatieres->item(row, 5);
        if (statusItem) {
            bool shouldShow = (selectedStatus == "All") || (statusItem->text() == selectedStatus);
            ui->tableMatieres->setRowHidden(row, !shouldShow);
        }
    }
}

void MainWindow::searchMaterials()
{
    QString searchText = ui->searchMatieres->text().toLower();
    
    for (int row = 0; row < ui->tableMatieres->rowCount(); ++row) {
        QTableWidgetItem* nameItem = ui->tableMatieres->item(row, 0);
        QTableWidgetItem* typeItem = ui->tableMatieres->item(row, 1);
        
        bool matchesSearch = (nameItem && nameItem->text().toLower().contains(searchText)) ||
                            (typeItem && typeItem->text().toLower().contains(searchText));
        
        bool isHidden = ui->tableMatieres->isRowHidden(row);
        
        if (matchesSearch && !isHidden) {
            ui->tableMatieres->setRowHidden(row, false);
        } else if (!matchesSearch) {
            ui->tableMatieres->setRowHidden(row, true);
        }
    }
}

// ============== COMMANDS TAB FUNCTIONS ==============

void MainWindow::initializeCommandsTab()
{
    // Setup Commands Table
    ui->tableCommands->setColumnCount(9);
    QStringList headers;
    headers << "ID" << "Type" << "Seller" << "Quantity" << "Order Date" << "Delivery Date" << "Status" << "Total Price" << "Email";
    ui->tableCommands->setHorizontalHeaderLabels(headers);
    ui->tableCommands->setColumnWidth(0, 60);
    ui->tableCommands->setColumnWidth(1, 80);
    ui->tableCommands->setColumnWidth(2, 100);
    ui->tableCommands->setColumnWidth(3, 70);
    ui->tableCommands->setColumnWidth(4, 100);
    ui->tableCommands->setColumnWidth(5, 100);
    ui->tableCommands->setColumnWidth(6, 90);
    ui->tableCommands->setColumnWidth(7, 90);
    ui->tableCommands->setColumnWidth(8, 120);
    
    // Connect buttons
    connect(ui->confirmCommandButton, &QPushButton::clicked, this, &MainWindow::addCommand);
    connect(ui->cancelCommandButton, &QPushButton::clicked, this, &MainWindow::clearCommandForm);
    connect(ui->deleteCommandButton, &QPushButton::clicked, this, &MainWindow::deleteCommand);
    connect(ui->exportCommandButton, &QPushButton::clicked, this, &MainWindow::exportCommands);
    connect(ui->statisticsCommandButton, &QPushButton::clicked, this, &MainWindow::showCommandStatistics);
    connect(ui->sendNotificationButton, &QPushButton::clicked, this, &MainWindow::sendNotification);
    connect(ui->comboFilterCommandStatus, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::filterCommands);
    connect(ui->comboSortBy, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::sortCommands);
    connect(ui->searchCommands, &QLineEdit::textChanged, this, &MainWindow::searchCommands);
    
    // Connect table row clicks to edit functionality
    connect(ui->tableCommands, &QTableWidget::cellClicked, this, &MainWindow::onCommandTableRowClicked);
}

void MainWindow::addCommand()
{
    QString id = ui->lineCommandID->text();
    QString type = ui->comboCommandType->currentText();
    QString seller = ui->lineCommandSeller->text();
    int quantity = ui->spinCommandQuantity->value();
    QString orderDate = ui->dateCommandOrder->date().toString("yyyy-MM-dd");
    QString deliveryDate = ui->dateCommandDelivery->date().toString("yyyy-MM-dd");
    QString status = ui->comboCommandStatus->currentText();
    double totalPrice = ui->spinCommandTotal->value();
    QString email = ui->lineCommandEmail->text();
    
    if (id.isEmpty() || seller.isEmpty() || email.isEmpty()) {
        qWarning() << "Please fill all required fields";
        return;
    }
    
    if (editingRow >= 0) {
        // Edit existing command
        editCommand();
        return;
    }
    
    int row = ui->tableCommands->rowCount();
    ui->tableCommands->insertRow(row);
    
    ui->tableCommands->setItem(row, 0, new QTableWidgetItem(id));
    ui->tableCommands->setItem(row, 1, new QTableWidgetItem(type));
    ui->tableCommands->setItem(row, 2, new QTableWidgetItem(seller));
    ui->tableCommands->setItem(row, 3, new QTableWidgetItem(QString::number(quantity)));
    ui->tableCommands->setItem(row, 4, new QTableWidgetItem(orderDate));
    ui->tableCommands->setItem(row, 5, new QTableWidgetItem(deliveryDate));
    ui->tableCommands->setItem(row, 6, new QTableWidgetItem(status));
    ui->tableCommands->setItem(row, 7, new QTableWidgetItem(QString::number(totalPrice, 'f', 2)));
    ui->tableCommands->setItem(row, 8, new QTableWidgetItem(email));
    
    clearCommandForm();
}

void MainWindow::clearCommandForm()
{
    ui->lineCommandID->clear();
    ui->comboCommandType->setCurrentIndex(0);
    ui->lineCommandSeller->clear();
    ui->spinCommandQuantity->setValue(0);
    ui->dateCommandOrder->setDate(QDate::currentDate());
    ui->dateCommandDelivery->setDate(QDate::currentDate());
    ui->comboCommandStatus->setCurrentIndex(0);
    ui->spinCommandTotal->setValue(0);
    ui->lineCommandEmail->clear();
}

void MainWindow::deleteCommand()
{
    int currentRow = ui->tableCommands->currentRow();
    if (currentRow >= 0) {
        ui->tableCommands->removeRow(currentRow);
        clearCommandForm();
        editingRow = -1;
    } else {
        qWarning() << "Please select a command to delete";
    }
}

void MainWindow::editCommand()
{
    if (editingRow < 0) return;
    
    QString type = ui->comboCommandType->currentText();
    QString seller = ui->lineCommandSeller->text();
    int quantity = ui->spinCommandQuantity->value();
    QString orderDate = ui->dateCommandOrder->date().toString("yyyy-MM-dd");
    QString deliveryDate = ui->dateCommandDelivery->date().toString("yyyy-MM-dd");
    QString status = ui->comboCommandStatus->currentText();
    double totalPrice = ui->spinCommandTotal->value();
    QString email = ui->lineCommandEmail->text();
    
    // Update the row in the table
    ui->tableCommands->setItem(editingRow, 1, new QTableWidgetItem(type));
    ui->tableCommands->setItem(editingRow, 2, new QTableWidgetItem(seller));
    ui->tableCommands->setItem(editingRow, 3, new QTableWidgetItem(QString::number(quantity)));
    ui->tableCommands->setItem(editingRow, 4, new QTableWidgetItem(orderDate));
    ui->tableCommands->setItem(editingRow, 5, new QTableWidgetItem(deliveryDate));
    ui->tableCommands->setItem(editingRow, 6, new QTableWidgetItem(status));
    ui->tableCommands->setItem(editingRow, 7, new QTableWidgetItem(QString::number(totalPrice, 'f', 2)));
    ui->tableCommands->setItem(editingRow, 8, new QTableWidgetItem(email));
    
    clearCommandForm();
    editingRow = -1;
}

void MainWindow::onCommandTableRowClicked(int row, int column)
{
    if (row < 0) return;
    
    editingRow = row;
    
    // Load data from table into form
    QString id = ui->tableCommands->item(row, 0)->text();
    QString type = ui->tableCommands->item(row, 1)->text();
    QString seller = ui->tableCommands->item(row, 2)->text();
    int quantity = ui->tableCommands->item(row, 3)->text().toInt();
    QString orderDate = ui->tableCommands->item(row, 4)->text();
    QString deliveryDate = ui->tableCommands->item(row, 5)->text();
    QString status = ui->tableCommands->item(row, 6)->text();
    double totalPrice = ui->tableCommands->item(row, 7)->text().toDouble();
    QString email = ui->tableCommands->item(row, 8)->text();
    
    // Fill form fields
    ui->lineCommandID->setText(id);
    ui->lineCommandID->setReadOnly(true);  // ID should not be editable
    ui->comboCommandType->setCurrentText(type);
    ui->lineCommandSeller->setText(seller);
    ui->spinCommandQuantity->setValue(quantity);
    ui->dateCommandOrder->setDate(QDate::fromString(orderDate, "yyyy-MM-dd"));
    ui->dateCommandDelivery->setDate(QDate::fromString(deliveryDate, "yyyy-MM-dd"));
    ui->comboCommandStatus->setCurrentText(status);
    ui->spinCommandTotal->setValue(totalPrice);
    ui->lineCommandEmail->setText(email);
}

void MainWindow::exportCommands()
{
    qDebug() << "Exporting commands to PDF...";
    // TODO: Implement PDF export with order details, statistics, and status distribution
}

void MainWindow::showCommandStatistics()
{
    // Calculate statistics
    int totalOrders = ui->tableCommands->rowCount();
    int pendingCount = 0;
    int inProductionCount = 0;
    int deliveredCount = 0;
    double totalRevenue = 0;
    
    for (int row = 0; row < ui->tableCommands->rowCount(); ++row) {
        QTableWidgetItem* statusItem = ui->tableCommands->item(row, 6);
        QTableWidgetItem* priceItem = ui->tableCommands->item(row, 7);
        
        if (statusItem) {
            if (statusItem->text() == "Pending") pendingCount++;
            else if (statusItem->text() == "In Production") inProductionCount++;
            else if (statusItem->text() == "Delivered") deliveredCount++;
        }
        
        if (priceItem) {
            totalRevenue += priceItem->text().toDouble();
        }
    }
    
    qDebug() << "Total Orders: " << totalOrders;
    qDebug() << "Pending: " << pendingCount;
    qDebug() << "In Production: " << inProductionCount;
    qDebug() << "Delivered: " << deliveredCount;
    qDebug() << "Total Revenue: " << totalRevenue;
}

void MainWindow::filterCommands()
{
    QString selectedStatus = ui->comboFilterCommandStatus->currentText();
    
    for (int row = 0; row < ui->tableCommands->rowCount(); ++row) {
        QTableWidgetItem* statusItem = ui->tableCommands->item(row, 6);
        if (statusItem) {
            bool shouldShow = (selectedStatus == "All") || (statusItem->text() == selectedStatus);
            ui->tableCommands->setRowHidden(row, !shouldShow);
        }
    }
}

void MainWindow::searchCommands()
{
    QString searchText = ui->searchCommands->text().toLower();
    
    for (int row = 0; row < ui->tableCommands->rowCount(); ++row) {
        QTableWidgetItem* idItem = ui->tableCommands->item(row, 0);
        QTableWidgetItem* typeItem = ui->tableCommands->item(row, 1);
        QTableWidgetItem* sellerItem = ui->tableCommands->item(row, 2);
        
        bool matchesSearch = (idItem && idItem->text().toLower().contains(searchText)) ||
                            (typeItem && typeItem->text().toLower().contains(searchText)) ||
                            (sellerItem && sellerItem->text().toLower().contains(searchText));
        
        bool isHidden = ui->tableCommands->isRowHidden(row);
        
        if (matchesSearch && !isHidden) {
            ui->tableCommands->setRowHidden(row, false);
        } else if (!matchesSearch) {
            ui->tableCommands->setRowHidden(row, true);
        }
    }
}

void MainWindow::sortCommands()
{
    QString sortBy = ui->comboSortBy->currentText();
    
    if (sortBy == "Date") {
        ui->tableCommands->sortItems(4);  // Order Date column
    } else if (sortBy == "Quantity") {
        ui->tableCommands->sortItems(3);  // Quantity column
    } else if (sortBy == "Status") {
        ui->tableCommands->sortItems(6);  // Status column
    }
}

void MainWindow::sendNotification()
{
    int currentRow = ui->tableCommands->currentRow();
    if (currentRow >= 0) {
        QTableWidgetItem* emailItem = ui->tableCommands->item(currentRow, 8);
        QTableWidgetItem* statusItem = ui->tableCommands->item(currentRow, 6);
        
        if (emailItem && statusItem) {
            QString email = emailItem->text();
            QString status = statusItem->text();
            qDebug() << "Sending notification to:" << email << "with status:" << status;
            // TODO: Implement email notification system
        }
    } else {
        qWarning() << "Please select a command to send notification";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
