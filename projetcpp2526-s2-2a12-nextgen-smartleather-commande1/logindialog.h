#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTimer>

class LoginDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    bool isAccepted() const { return m_accepted; }
    QString loggedEmail() const { return m_loggedEmail; }

private slots:
    void onLogin();
    void onFaceIdLogin();
    void onEnrollFaceId();
    void onCreateAccount();
    void onForgotPassword();
    void updateLockUI();

private:
    QLineEdit *m_userEdit = nullptr;
    QLineEdit *m_passEdit = nullptr;
    QLabel *m_errorLabel = nullptr;
    QLabel *m_lockLabel = nullptr;
    QPushButton *m_btnLogin = nullptr;
    QPushButton *m_btnFaceIdLogin = nullptr;
    QPushButton *m_btnEnrollFaceId = nullptr;
    QPushButton *m_btnCreateAccount = nullptr;
    QPushButton *m_btnForgotPassword = nullptr;
    QTimer *m_lockTimer = nullptr;

    bool m_accepted = false;
    QString m_loggedEmail;
    int m_failedAttempts = 0;
    static constexpr int kMaxAttempts = 3;
    static constexpr int kFaceHashThreshold = 10;

    void setInputsEnabled(bool enabled);
    bool validateCredentials(const QString &user, const QString &pass);
    bool emailExists(const QString &email) const;
    QString computeFaceHash(const QString &imagePath) const;
    int hexHammingDistance(const QString &leftHex, const QString &rightHex) const;
    void setupUI();
};

#endif // LOGINDIALOG_H
