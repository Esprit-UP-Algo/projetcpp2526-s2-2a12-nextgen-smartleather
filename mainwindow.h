#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include <QMap>
#include <QTextCharFormat>
#include <QTableWidget>
#include <QDateTime>
#include <QList>
#include "smtp.h"
#include <QLineEdit>
#include <QComboBox>

struct EmailHistory {
    QDateTime dateTime;
    QString to;
    QString subject;
    QString body;
    QString type;
    QString orderID;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}

class MaterialsWindow;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
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
    // void sendEmailToClient(); // Désactivé - envoi automatique uniquement
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
    QMap<QDate, QStringList> deliveryDates; // Date -> List of order IDs
    QMap<QDate, QMap<QString, QString>> deliveryStatuses; // Date -> (OrderID -> Status)
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
};
#endif // MAINWINDOW_H
