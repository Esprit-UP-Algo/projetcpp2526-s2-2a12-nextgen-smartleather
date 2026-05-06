#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include <QMap>
#include <QTextCharFormat>
#include <QTableWidget>
#include <QDateTime>
#include <QList>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QStackedWidget>
#include <QPushButton>
#include <QDateEdit>
#include <QStandardItemModel>
#include "smtp.h"

struct EmailHistory {
    QDateTime dateTime;
    QString to;
    QString subject;
    QString body;
    QString type;
    QString orderID;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MaterialsWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString &loggedEmail = QString(), QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCalendarDateChanged();
    void onExportCalendar();
    void updateStatistics();
    void updateCalendarHighlights();
    void displayOrdersForUpdate();
    void applyAdvancedFilters();
    void clearAdvancedFilters();
    void checkForNotifications();
    void displayEmailHistory();
    void resendEmailFromHistory();
    void clearEmailHistory();
    void filterEmailHistory();
    void sortEmailHistory(int column);
    void searchOrdersList();
    void sortOrdersList();
    void exportListToPDF();

private:
    Ui::MainWindow *ui;
    QMap<QDate, QStringList> deliveryDates;
    QMap<QDate, QMap<QString, QString>> deliveryStatuses;
    QTableWidget *table_update = nullptr;
    QTableWidget *table_email_history = nullptr;
    QLineEdit *search_email_history = nullptr;
    QComboBox *filter_email_type = nullptr;
    QList<EmailHistory> emailHistory;
    int currentSortColumn = 0;
    Qt::SortOrder currentSortOrder = Qt::DescendingOrder;
    MaterialsWindow *materialsWindow = nullptr;

    // Configuration SMTP
    QString smtpServer = "smtp.gmail.com";
    int smtpPort = 587;
    QString smtpUsername;
    QString smtpPassword;
    QString m_loggedEmail;

    void updateDeliveryList(const QDate &date);
    void updateCalendarStats();
    void reloadOrdersFromDb();
    void exportToPDF(const QString &fileName);
    void exportToCSV(const QString &fileName);
    QString getEmailTemplate(const QString &templateName, const QMap<QString, QString> &data);
    bool sendEmail(const QString &to, const QString &subject, const QString &body);
    bool sendEmailSMTP(const QString &to, const QString &subject, const QString &body);
    void addToEmailHistory(const QString &to, const QString &subject, const QString &body, const QString &type, const QString &orderID);
    void loadEmailHistory();
    void saveEmailHistory();

    // ── Employees ──────────────────────────────────────────────
    void setupEmployeeUI();
    void applyBarStyle(const QList<QPushButton*> &buttons, bool checkable);

    QStandardItemModel *employeeModel   = nullptr;
    QTableWidget       *employeesTable  = nullptr;
    QLineEdit          *empIdAdd        = nullptr;
    QLineEdit          *empNameAdd      = nullptr;
    QLineEdit          *empAddrAdd      = nullptr;
    QLineEdit          *empEmailAdd     = nullptr;
    QLineEdit          *empPhoneAdd     = nullptr;
    QComboBox          *empPosteAdd     = nullptr;
    QComboBox          *empSexAdd       = nullptr;
    QDoubleSpinBox     *empSalaryAdd    = nullptr;
    QDateEdit          *empHireDateAdd  = nullptr;
    QLineEdit          *empSearchEdit   = nullptr;
    QLineEdit          *empEditName     = nullptr;
    QLineEdit          *empEditPhone    = nullptr;
    QDoubleSpinBox     *empEditSalary   = nullptr;
    QLineEdit          *empDeleteId     = nullptr;
    QLabel             *empStatsTotal   = nullptr;
    QLabel             *empStatsAvgSalary = nullptr;
    QLabel             *empStatsHommes  = nullptr;
    QLabel             *empStatsFemmes  = nullptr;
    QLabel             *empStatsSalMin  = nullptr;
    QLabel             *empStatsSalMax  = nullptr;
    QLabel             *empStatsPoste   = nullptr;
    QStackedWidget     *employeeStack   = nullptr;
    QList<QPushButton*> employeeTabButtons;
};

#endif // MAINWINDOW_H
