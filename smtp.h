#ifndef SMTP_H
#define SMTP_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QSslSocket>

class SmtpClient : public QObject
{
    Q_OBJECT

public:
    explicit SmtpClient(QObject *parent = nullptr);
    ~SmtpClient();
    
    void sendEmail(const QString &from, const QString &to, 
                   const QString &subject, const QString &body,
                   const QString &smtpServer = "smtp.gmail.com",
                   int smtpPort = 587,
                   const QString &username = "",
                   const QString &password = "");

signals:
    void emailSent(bool success, const QString &message);

private slots:
    void onConnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError error);

private:
    QSslSocket *socket;
    QString from;
    QString to;
    QString subject;
    QString body;
    QString username;
    QString password;
    
    enum State {
        Init,
        HandShake,
        Auth,
        User,
        Pass,
        Mail,
        Rcpt,
        Data,
        Body,
        Quit,
        Close
    };
    
    State state;
    QString response;
    bool messageAccepted;
    bool resultEmitted;
    
    void sendCommand(const QString &command);
};

#endif // SMTP_H
