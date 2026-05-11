#include "emailservice.h"
#include <QDebug>
#include <QEventLoop>
#include <QThread>

EmailService::EmailService(QObject *parent)
    : QObject(parent), smtpPort(587), smtpSocket(nullptr),
      isEmailSent(false), smtpStep(0)
{
    smtpServer = "smtp.gmail.com";
    fromEmail = "noreply@smartleather.com";
    fromName = "SmartLeather";

    timeoutTimer = new QTimer(this);
    timeoutTimer->setSingleShot(true);
    timeoutTimer->setInterval(30000);
    connect(timeoutTimer, &QTimer::timeout, this, &EmailService::onSmtpTimeout);
}

void EmailService::setSmtpServer(const QString &server, int port)
{
    smtpServer = server;
    smtpPort = port;
}

void EmailService::setSmtpCredentials(const QString &username, const QString &password)
{
    smtpUsername = username;
    smtpPassword = password;
}

void EmailService::setFromEmail(const QString &email, const QString &name)
{
    fromEmail = email;
    if (!name.isEmpty()) fromName = name;
}

QString EmailService::getLastError() const { return lastError; }

bool EmailService::sendPasswordResetEmail(const QString &toEmail, const QString &nom,
                                          const QString &prenom, const QString &newPassword)
{
    if (toEmail.isEmpty() || !toEmail.contains("@")) {
        lastError = "Adresse email invalide";
        return false;
    }

    QString body = QString(
        "Bonjour %1 %2,\n\n"
        "Votre nouveau mot de passe est :\n\n"
        "    %3\n\n"
        "Veuillez vous connecter et changer votre mot de passe.\n\n"
        "Cordialement,\nSmartLeather"
    ).arg(prenom, nom, newPassword);

    return sendViaSmtp(toEmail, "SmartLeather - Reinitialisation mot de passe", body);
}

bool EmailService::sendViaSmtp(const QString &to, const QString &subject, const QString &body)
{
    if (smtpServer.isEmpty() || smtpUsername.isEmpty() || smtpPassword.isEmpty()) {
        lastError = "SMTP non configure";
        return false;
    }

    if (smtpSocket) {
        smtpSocket->disconnectFromHost();
        smtpSocket->deleteLater();
        smtpSocket = nullptr;
    }

    smtpSocket = new QSslSocket(this);
    connect(smtpSocket, &QSslSocket::readyRead, this, &EmailService::onSmtpReadyRead);
    connect(smtpSocket, &QSslSocket::sslErrors, this, &EmailService::onSslErrors);
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    connect(smtpSocket, &QSslSocket::errorOccurred, this, &EmailService::onSmtpError);
#else
    connect(smtpSocket, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(
                &QAbstractSocket::error), this, &EmailService::onSmtpError);
#endif

    currentTo = to;
    currentSubject = subject;
    currentMessage = body;
    isEmailSent = false;
    smtpStep = 0;
    lastError = "";

    if (smtpPort == 465) {
        smtpSocket->connectToHostEncrypted(smtpServer, smtpPort);
        if (!smtpSocket->waitForConnected(15000) || !smtpSocket->waitForEncrypted(20000)) {
            lastError = "Connexion SSL echouee: " + smtpSocket->errorString();
            return false;
        }
        if (!smtpSocket->waitForReadyRead(10000)) {
            lastError = "Timeout attente serveur";
            return false;
        }
        smtpStep = 3;
    } else {
        smtpSocket->connectToHost(smtpServer, smtpPort);
        if (!smtpSocket->waitForConnected(15000)) {
            lastError = "Connexion echouee: " + smtpSocket->errorString();
            return false;
        }
        if (!smtpSocket->waitForReadyRead(5000)) {
            lastError = "Timeout attente serveur";
            return false;
        }
    }

    timeoutTimer->start();

    QEventLoop loop;
    QMetaObject::Connection c1 = connect(this, &EmailService::emailSent, &loop, &QEventLoop::quit);
    QMetaObject::Connection c2 = connect(timeoutTimer, &QTimer::timeout, &loop, &QEventLoop::quit);
    loop.exec();
    disconnect(c1);
    disconnect(c2);
    timeoutTimer->stop();

    return isEmailSent;
}

void EmailService::sendCmd(const QString &cmd)
{
    if (smtpSocket && smtpSocket->state() == QAbstractSocket::ConnectedState) {
        qDebug() << "SMTP >>" << cmd;
        smtpSocket->write((cmd + "\r\n").toUtf8());
    }
}

void EmailService::processResponse(const QString &response)
{
    qDebug() << "SMTP <<" << response;

    if (response.startsWith("220")) {
        if (smtpStep == 0) {
            smtpStep = 1;
            sendCmd("EHLO localhost");
        } else if (smtpStep == 2) {
            smtpSocket->startClientEncryption();
            if (!smtpSocket->waitForEncrypted(20000)) {
                lastError = "SSL echoue: " + smtpSocket->errorString();
                isEmailSent = false;
                emit emailSent();
                return;
            }
            smtpStep = 3;
            sendCmd("EHLO localhost");
        }
    } else if (response.startsWith("250")) {
        if (smtpStep == 1) {
            if (smtpPort == 587 && response.contains("STARTTLS", Qt::CaseInsensitive)) {
                smtpStep = 2;
                sendCmd("STARTTLS");
            } else {
                smtpStep = 4;
                sendCmd("AUTH LOGIN");
            }
        } else if (smtpStep == 3) {
            smtpStep = 4;
            sendCmd("AUTH LOGIN");
        } else if (smtpStep == 6) {
            smtpStep = 7;
            sendCmd(QString("RCPT TO:<%1>").arg(currentTo));
        } else if (smtpStep == 7) {
            smtpStep = 8;
            sendCmd("DATA");
        } else if (smtpStep == 9) {
            isEmailSent = true;
            smtpStep = 10;
            sendCmd("QUIT");
            emit emailSent();
        }
    } else if (response.startsWith("334")) {
        if (smtpStep == 4) {
            smtpStep = 5;
            sendCmd(QString::fromUtf8(smtpUsername.toUtf8().toBase64()));
        } else if (smtpStep == 5) {
            sendCmd(QString::fromUtf8(smtpPassword.toUtf8().toBase64()));
        }
    } else if (response.startsWith("235")) {
        smtpStep = 6;
        sendCmd(QString("MAIL FROM:<%1>").arg(fromEmail));
    } else if (response.startsWith("354")) {
        smtpStep = 9;
        QString data = QString(
            "From: %1 <%2>\r\n"
            "To: %3\r\n"
            "Subject: %4\r\n"
            "MIME-Version: 1.0\r\n"
            "Content-Type: text/plain; charset=UTF-8\r\n"
            "\r\n"
            "%5\r\n"
            ".\r\n"
        ).arg(fromName, fromEmail, currentTo, currentSubject, currentMessage);
        smtpSocket->write(data.toUtf8());
        smtpSocket->flush();
    } else if (response.startsWith("4") || response.startsWith("5")) {
        lastError = "Erreur SMTP: " + response;
        isEmailSent = false;
        emit emailSent();
    }
}

void EmailService::onSmtpReadyRead()
{
    QString data = QString::fromUtf8(smtpSocket->readAll());
    for (const QString &line : data.split("\r\n", Qt::SkipEmptyParts)) {
        if (!line.trimmed().isEmpty())
            processResponse(line.trimmed());
    }
}

void EmailService::onSmtpError(QAbstractSocket::SocketError)
{
    lastError = "Erreur socket: " + smtpSocket->errorString();
    isEmailSent = false;
    emit emailSent();
}

void EmailService::onSslErrors(const QList<QSslError> &)
{
    smtpSocket->ignoreSslErrors();
}

void EmailService::onSmtpTimeout()
{
    lastError = "Timeout SMTP";
    isEmailSent = false;
    emit emailSent();
}
