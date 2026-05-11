#include "employe.h"
#include "ui_employe.h"
#include "connection.h"

#include <QApplication>
#include <QDate>
#include <QHeaderView>
#include <QIntValidator>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTableWidgetItem>

namespace {
const QRegularExpression &phonePattern()
{
    static const QRegularExpression pattern(
        "^\\+?[0-9][0-9\\s\\-]{6,18}$"
    );
    return pattern;
}

bool isPositiveIntegerText(const QString &text)
{
    bool ok = false;
    const int value = text.trimmed().toInt(&ok);
    return ok && value > 0;
}

bool isValidPersonName(const QString &name)
{
    const QString trimmed = name.trimmed();
    if (trimmed.isEmpty()) {
        return false;
    }

    static const QRegularExpression pattern("^[A-Za-zÀ-ÖØ-öø-ÿ' -]{2,80}$");
    return pattern.match(trimmed).hasMatch();
}

bool isValidPhone(const QString &phone)
{
    const QString trimmed = phone.trimmed();
    if (trimmed.isEmpty() || !phonePattern().match(trimmed).hasMatch()) {
        return false;
    }

    int digitCount = 0;
    for (const QChar ch : trimmed) {
        if (ch.isDigit()) {
            ++digitCount;
        }
    }
    return digitCount >= 8 && digitCount <= 15;
}
}

EmployeWindow::EmployeWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EmployeWindow)
{
    ui->setupUi(this);

    if (ui->tableEmployees) {
        ui->tableEmployees->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableEmployees->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableEmployees->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableEmployees->setAlternatingRowColors(true);
    }

    if (ui->cinAdd) {
        ui->cinAdd->setValidator(new QIntValidator(1, 999999999, this));
        ui->cinAdd->setMaxLength(12);
    }
    if (ui->cinSearch) {
        ui->cinSearch->setValidator(new QIntValidator(1, 999999999, this));
        ui->cinSearch->setMaxLength(12);
    }
    if (ui->cinDelete) {
        ui->cinDelete->setValidator(new QIntValidator(1, 999999999, this));
        ui->cinDelete->setMaxLength(12);
    }
    if (ui->phoneAdd) {
        ui->phoneAdd->setValidator(new QRegularExpressionValidator(phonePattern(), this));
        ui->phoneAdd->setMaxLength(20);
    }
    if (ui->phoneEdit) {
        ui->phoneEdit->setValidator(new QRegularExpressionValidator(phonePattern(), this));
        ui->phoneEdit->setMaxLength(20);
    }
    if (ui->nameAdd) {
        ui->nameAdd->setMaxLength(80);
    }
    if (ui->nameEdit) {
        ui->nameEdit->setMaxLength(80);
    }
    if (ui->addressAdd) {
        ui->addressAdd->setMaxLength(160);
    }

    connect(ui->btnTabAdd, &QPushButton::clicked, this, [this]() { setPage(0); });
    connect(ui->btnTabEdit, &QPushButton::clicked, this, [this]() { setPage(1); });
    connect(ui->btnTabDelete, &QPushButton::clicked, this, [this]() { setPage(2); });
    connect(ui->btnTabList, &QPushButton::clicked, this, [this]() {
        setPage(3);
        refreshEmployees();
    });

    connect(ui->btnAdd, &QPushButton::clicked, this, &EmployeWindow::addEmployee);
    connect(ui->btnLoad, &QPushButton::clicked, this, &EmployeWindow::loadEmployee);
    connect(ui->btnUpdate, &QPushButton::clicked, this, &EmployeWindow::updateEmployee);
    connect(ui->btnDelete, &QPushButton::clicked, this, &EmployeWindow::deleteEmployee);
    connect(ui->btnRefresh, &QPushButton::clicked, this, &EmployeWindow::refreshEmployees);
    connect(ui->btnResetAdd, &QPushButton::clicked, this, &EmployeWindow::clearAddForm);

    setPage(0);
    refreshEmployees();
}

EmployeWindow::~EmployeWindow()
{
    delete ui;
}

bool EmployeWindow::ensureDatabase()
{
    QSqlDatabase db = Connection::instance()->database();
    if (!db.isOpen()) {
        Connection::instance()->createConnect();
        db = Connection::instance()->database();
    }
    return db.isValid() && db.isOpen();
}

void EmployeWindow::setPage(int index)
{
    if (ui->stackedWidget) {
        ui->stackedWidget->setCurrentIndex(index);
    }

    const QList<QPushButton*> buttons = {
        ui->btnTabAdd,
        ui->btnTabEdit,
        ui->btnTabDelete,
        ui->btnTabList
    };
    for (int i = 0; i < buttons.size(); ++i) {
        if (buttons[i]) {
            buttons[i]->setChecked(i == index);
        }
    }
}

void EmployeWindow::clearAddForm()
{
    if (ui->cinAdd) ui->cinAdd->clear();
    if (ui->nameAdd) ui->nameAdd->clear();
    if (ui->addressAdd) ui->addressAdd->clear();
    if (ui->phoneAdd) ui->phoneAdd->clear();
    if (ui->salaryAdd) ui->salaryAdd->setValue(0.0);
    if (ui->dateAdd) ui->dateAdd->setDate(QDate::currentDate());
    if (ui->posteAdd) ui->posteAdd->setCurrentIndex(0);
    if (ui->sexAdd) ui->sexAdd->setCurrentIndex(0);
}

void EmployeWindow::clearEditForm()
{
    if (ui->nameEdit) ui->nameEdit->clear();
    if (ui->phoneEdit) ui->phoneEdit->clear();
    if (ui->salaryEdit) ui->salaryEdit->setValue(0.0);
    m_loadedCin.clear();
}

void EmployeWindow::clearDeleteForm()
{
    if (ui->cinDelete) ui->cinDelete->clear();
}

void EmployeWindow::updateStats(int totalRows, double salarySum)
{
    if (ui->labelTotal) {
        ui->labelTotal->setText(QString::number(totalRows));
    }
    if (ui->labelAverageSalary) {
        const double average = totalRows > 0 ? salarySum / totalRows : 0.0;
        ui->labelAverageSalary->setText(QString::number(average, 'f', 2));
    }
}

void EmployeWindow::populateTable()
{
    if (!ui->tableEmployees) {
        return;
    }

    ui->tableEmployees->setRowCount(0);

    if (!ensureDatabase()) {
        QMessageBox::critical(this, "Base de donnees", "Connexion a la base de donnees indisponible.");
        updateStats(0, 0.0);
        return;
    }

    QSqlDatabase db = Connection::instance()->database();
    QSqlQuery query(db);
    query.prepare("SELECT CIN, NOM, POSTE, ADRESSE, TELEPHONE, DATE_EMBAUCHE, SALAIRE, STATUT, SEXE FROM EMPLOYE ORDER BY CIN");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", query.lastError().text());
        updateStats(0, 0.0);
        return;
    }

    double salarySum = 0.0;
    int row = 0;
    while (query.next()) {
        ui->tableEmployees->insertRow(row);
        for (int col = 0; col < 9; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            ui->tableEmployees->setItem(row, col, item);
        }
        salarySum += query.value(6).toDouble();
        ++row;
    }

    updateStats(row, salarySum);
}

void EmployeWindow::refreshEmployees()
{
    populateTable();
}

void EmployeWindow::addEmployee()
{
    const QString cin = ui->cinAdd ? ui->cinAdd->text().trimmed() : QString();
    const QString name = ui->nameAdd ? ui->nameAdd->text().trimmed() : QString();
    const QString address = ui->addressAdd ? ui->addressAdd->text().trimmed() : QString();
    const QString phone = ui->phoneAdd ? ui->phoneAdd->text().trimmed() : QString();
    const QDate hireDate = ui->dateAdd ? ui->dateAdd->date() : QDate::currentDate();
    const QString poste = ui->posteAdd ? ui->posteAdd->currentText() : QString();
    const double salary = ui->salaryAdd ? ui->salaryAdd->value() : 0.0;
    const QString sex = ui->sexAdd ? ui->sexAdd->currentText() : QString();

    if (!isPositiveIntegerText(cin)) {
        QMessageBox::warning(this, "Saisie invalide", "Le CIN doit etre un entier positif.");
        return;
    }
    if (!isValidPersonName(name)) {
        QMessageBox::warning(this, "Saisie invalide", "Le nom employe est invalide.");
        return;
    }
    if (!phone.isEmpty() && !isValidPhone(phone)) {
        QMessageBox::warning(this, "Saisie invalide", "Le telephone employe est invalide.");
        return;
    }
    if (!hireDate.isValid() || hireDate > QDate::currentDate()) {
        QMessageBox::warning(this, "Saisie invalide", "La date d'embauche ne peut pas etre dans le futur.");
        return;
    }
    if (salary <= 0.0) {
        QMessageBox::warning(this, "Saisie invalide", "Le salaire doit etre superieur a 0.");
        return;
    }

    if (!ensureDatabase()) {
        QMessageBox::critical(this, "Base de donnees", "Connexion a la base de donnees indisponible.");
        return;
    }

    QSqlDatabase db = Connection::instance()->database();
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT 1 FROM EMPLOYE WHERE CIN = :cin");
    checkQuery.bindValue(":cin", cin);
    if (checkQuery.exec() && checkQuery.next()) {
        QMessageBox::warning(this, "Doublon", "Un employe avec ce CIN existe deja.");
        return;
    }

    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO EMPLOYE (CIN, NOM, POSTE, ADRESSE, TELEPHONE, DATE_EMBAUCHE, SALAIRE, STATUT, SEXE) "
        "VALUES (:cin, :nom, :poste, :adresse, :telephone, :date_embauche, :salaire, :statut, :sexe)"
    );
    query.bindValue(":cin", cin);
    query.bindValue(":nom", name);
    query.bindValue(":poste", poste);
    query.bindValue(":adresse", address);
    query.bindValue(":telephone", phone);
    query.bindValue(":date_embauche", hireDate);
    query.bindValue(":salaire", salary);
    query.bindValue(":statut", QStringLiteral("Actif"));
    query.bindValue(":sexe", sex);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", query.lastError().text());
        return;
    }

    clearAddForm();
    refreshEmployees();
    QMessageBox::information(this, "Succes", "Employe ajoute avec succes.");
}

void EmployeWindow::loadEmployee()
{
    const QString cin = ui->cinSearch ? ui->cinSearch->text().trimmed() : QString();
    if (!isPositiveIntegerText(cin)) {
        QMessageBox::warning(this, "Saisie invalide", "Le CIN doit etre un entier positif.");
        return;
    }

    if (!ensureDatabase()) {
        QMessageBox::critical(this, "Base de donnees", "Connexion a la base de donnees indisponible.");
        return;
    }

    QSqlDatabase db = Connection::instance()->database();
    QSqlQuery query(db);
    query.prepare("SELECT NOM, TELEPHONE, SALAIRE FROM EMPLOYE WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", query.lastError().text());
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Introuvable", "Employe non trouve.");
        return;
    }

    m_loadedCin = cin;
    if (ui->nameEdit) ui->nameEdit->setText(query.value(0).toString());
    if (ui->phoneEdit) ui->phoneEdit->setText(query.value(1).toString());
    if (ui->salaryEdit) ui->salaryEdit->setValue(query.value(2).toDouble());
    setPage(1);
}

void EmployeWindow::updateEmployee()
{
    const QString cin = m_loadedCin.isEmpty() && ui->cinSearch ? ui->cinSearch->text().trimmed() : m_loadedCin;
    const QString name = ui->nameEdit ? ui->nameEdit->text().trimmed() : QString();
    const QString phone = ui->phoneEdit ? ui->phoneEdit->text().trimmed() : QString();
    const double salary = ui->salaryEdit ? ui->salaryEdit->value() : 0.0;

    if (!isPositiveIntegerText(cin)) {
        QMessageBox::warning(this, "Saisie invalide", "Chargez d'abord un employe valide.");
        return;
    }
    if (!isValidPersonName(name)) {
        QMessageBox::warning(this, "Saisie invalide", "Le nom employe est invalide.");
        return;
    }
    if (!phone.isEmpty() && !isValidPhone(phone)) {
        QMessageBox::warning(this, "Saisie invalide", "Le telephone employe est invalide.");
        return;
    }
    if (salary <= 0.0) {
        QMessageBox::warning(this, "Saisie invalide", "Le salaire doit etre superieur a 0.");
        return;
    }

    if (!ensureDatabase()) {
        QMessageBox::critical(this, "Base de donnees", "Connexion a la base de donnees indisponible.");
        return;
    }

    QSqlDatabase db = Connection::instance()->database();
    QSqlQuery query(db);
    query.prepare("UPDATE EMPLOYE SET NOM = :nom, TELEPHONE = :telephone, SALAIRE = :salaire WHERE CIN = :cin");
    query.bindValue(":nom", name);
    query.bindValue(":telephone", phone);
    query.bindValue(":salaire", salary);
    query.bindValue(":cin", cin);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", query.lastError().text());
        return;
    }

    clearEditForm();
    refreshEmployees();
    QMessageBox::information(this, "Succes", "Employe mis a jour.");
}

void EmployeWindow::deleteEmployee()
{
    const QString cin = ui->cinDelete ? ui->cinDelete->text().trimmed() : QString();
    if (!isPositiveIntegerText(cin)) {
        QMessageBox::warning(this, "Saisie invalide", "Le CIN doit etre un entier positif.");
        return;
    }

    if (QMessageBox::question(this, "Confirmation", "Supprimer cet employe ?") != QMessageBox::Yes) {
        return;
    }

    if (!ensureDatabase()) {
        QMessageBox::critical(this, "Base de donnees", "Connexion a la base de donnees indisponible.");
        return;
    }

    QSqlDatabase db = Connection::instance()->database();
    QSqlQuery query(db);
    query.prepare("DELETE FROM EMPLOYE WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur", query.lastError().text());
        return;
    }

    clearDeleteForm();
    refreshEmployees();
    QMessageBox::information(this, "Succes", "Employe supprime.");
}
