#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class LoginDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    bool isAccepted() const { return m_accepted; }

private slots:
    void onLogin();
    void onCreateAccount();

private:
    QLineEdit   *m_userEdit;
    QLineEdit   *m_passEdit;
    QLabel      *m_errorLabel;
    bool         m_accepted = false;

    bool validateCredentials(const QString &user, const QString &pass);
};

#endif // LOGINDIALOG_H
