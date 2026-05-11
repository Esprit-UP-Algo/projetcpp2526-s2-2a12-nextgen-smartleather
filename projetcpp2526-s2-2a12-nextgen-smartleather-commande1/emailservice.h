#ifndef EMAILSERVICE_H
#define EMAILSERVICE_H

#include <QObject>
#include <QString>
#include <QSslSocket>
#include <QTimer>

class EmailService : public QObject
{
    Q_OBJECT

public:
    explicit EmailService(QObject *parent = nullptr);

    bool sendPasswordResetEmail(const QString &toEmail, const QString &nom,
                                const QString &prenom, const QString &newPassword);

    void setSmtpServer(const QString &server, int port = 587);
    void setSmtpCredentials(const QString &username, const QString &password);
    void setFromEmail(const QString &email, const QString &name = "");

    QString getLastError() const;

signals:
    void emailSent();

private slots:
    void onSmtpReadyRead();
    void onSmtpError(QAbstractSocket::SocketError error);
    void onSslErrors(const QList<QSslError> &errors);
    void onSmtpTimeout();

private:
    QString smtpServer;
    int smtpPort;
    QString smtpUsername;
    QString smtpPassword;
    QString fromEmail;
    QString fromName;
    QString lastError;

    QSslSocket *smtpSocket;
    QTimer *timeoutTimer;
    QString currentTo;
    QString currentSubject;
    QString currentMessage;
    bool isEmailSent;
    int smtpStep;

    bool sendViaSmtp(const QString &to, const QString &subject, const QString &body);
    void sendCmd(const QString &cmd);
    void processResponse(const QString &response);
};

#endif // EMAILSERVICE_H
