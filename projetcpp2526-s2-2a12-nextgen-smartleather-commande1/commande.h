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
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardItemModel>
#include <QDateEdit>
#include <QStackedWidget>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include "animationutils.h"
#include "buttonanimation.h"
#include "arduinosensor.h"

struct EmailHistory {
    QDateTime dateTime;
    QString to;
    QString subject;
    QString body;
    QString type;
    QString orderID;
};

struct Supplier {
    QString id;
    QString nom;
    QString type;
    QString adresse;
    QString telephone;
    QString email;
    int delai = 0;
    double prix = 0.0;
    QString statut;
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
    
    // Arduino Sensor slots
    void onArduinoTemperatureReceived(float temperature, float humidity);
    void onArduinoErrorOccurred(const QString &error);
    void onArduinoConnectedStatusChanged(bool connected);
    void connectToArduino();
    void disconnectFromArduino();
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
    QString m_loggedEmail;
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

    // Suppliers & Employees UIs (imported from legacy project)
    void setupSupplierUI();
    void setupEmployeeUI();
    Supplier supplierFromForm(bool *ok) const;
    void fillSupplierForm(const Supplier &supplier);
    void clearSupplierForm();
    void refreshSupplierTable();

    // Chat Bot
    void appendAiMessage(const QString &speaker, const QString &text);
    void sendAiMessage(const QString &userText);
    void sendGeminiMessage(const QString &userText);

    bool addSupplier(const Supplier &supplier);
    bool updateSupplier(const QString &id, const Supplier &supplier);
    bool deleteSupplier(const QString &id);
    Supplier *findSupplier(const QString &id);

    // Suppliers widgets
    QLineEdit *supIdInput = nullptr;
    QLineEdit *supNameInput = nullptr;
    QLineEdit *supTypeInput = nullptr;
    QLineEdit *supAddressInput = nullptr;
    QLineEdit *supPhoneInput = nullptr;
    QLineEdit *supEmailInput = nullptr;
    QSpinBox *supDelayInput = nullptr;
    QDoubleSpinBox *supPriceInput = nullptr;
    QComboBox *supStatusInput = nullptr;
    QTableWidget *suppliersTable = nullptr;
    QLabel *supStatTotal = nullptr;
    QLabel *supStatActive = nullptr;
    QLabel *supStatAvgPrice = nullptr;
    QLabel *supStatAvgDelay = nullptr;
    QString supplierLoadedId;
    Supplier currentSupplier;
    QStackedWidget *supplierStack = nullptr;
    QList<QPushButton*> supplierTabButtons;

    // Employees
    QStandardItemModel *employeeModel = nullptr;
    QTableWidget *employeesTable = nullptr;
    QLineEdit *empIdAdd = nullptr;
    QLineEdit *empNameAdd = nullptr;
    QLineEdit *empAddrAdd = nullptr;
    QLineEdit *empEmailAdd = nullptr;
    QLineEdit *empPhoneAdd = nullptr;
    QComboBox *empPosteAdd = nullptr;
    QComboBox *empSexAdd = nullptr;
    QDoubleSpinBox *empSalaryAdd = nullptr;
    QDateEdit *empHireDateAdd = nullptr;
    QLineEdit *empSearchEdit = nullptr;
    QLineEdit *empEditName = nullptr;
    QLineEdit *empEditPhone = nullptr;
    QDoubleSpinBox *empEditSalary = nullptr;
    QLineEdit *empDeleteId = nullptr;
    QLabel *empStatsTotal = nullptr;
    QLabel *empStatsAvgSalary = nullptr;
    QStackedWidget *employeeStack = nullptr;
    QList<QPushButton*> employeeTabButtons;

    void applyBarStyle(const QList<QPushButton*> &buttons, bool checkable);

    QNetworkAccessManager *aiNetwork = nullptr;
    QString geminiApiKey;
    QString geminiPreferredModel;
    QString buttonBarStyle;

    // Animation methods
    void animateFadeInWidget(QWidget* widget, int duration = 500);
    void animateFadeOutWidget(QWidget* widget, int duration = 500);
    void animateSlideInWidget(QWidget* widget, bool fromLeft = true, int duration = 400);
    void animatePulseWidget(QWidget* widget, int duration = 800);
    void animateStatisticsUpdate();
    void animateTableItemAppearance(QTableWidget* table);

    // Arduino Sensor - Variables privées
    void saveSensorDataToDatabase(float temperature, float humidity);
    
    ArduinoSensor *arduinoSensor = nullptr;
    QTimer *arduinoAutoConnectTimer = nullptr;
    float currentTemperature = 0.0f;
    float currentHumidity = 0.0f;
    float temperatureThreshold = 30.0f;  // Seuil d'alerte en °C (à modifier selon besoin)
    bool temperatureAlertShown = false;  // Éviter l'affichage multiple
    bool arduinoConnectedPopupShown = false;
    bool arduinoUnavailablePopupShown = false;
    bool sensorDbUnavailableWarningShown = false;
    QString lastArduinoPort;
    
    // Méthodes pour gérer les alertes de température
    void checkTemperatureThreshold(float temperature);
    void showTemperatureAlert(float temperature);
};
#endif // MAINWINDOW_H
