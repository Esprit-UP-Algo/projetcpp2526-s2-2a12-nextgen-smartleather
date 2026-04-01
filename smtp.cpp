#include "smtp.h"
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDir>

static void writeLog(const QString &message) {
    QFile logFile("smtp_debug.log");
    if (logFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&logFile);
        out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " - " << message << "\n";
        logFile.close();
    }
    qDebug() << message;
}

SmtpClient::SmtpClient(QObject *parent)
    : QObject(parent), socket(nullptr), state(Init), messageAccepted(false), resultEmitted(false)
{
}

SmtpClient::~SmtpClient()
{
    if (socket) {
        socket->close();
        socket->deleteLater();
    }
}

void SmtpClient::sendEmail(const QString &from, const QString &to,
                           const QString &subject, const QString &body,
                           const QString &smtpServer, int smtpPort,
                           const QString &username, const QString &password)
{
    this->from = from;
    this->to = to;
    this->subject = subject;
    this->body = body;
    this->username = username.isEmpty() ? from : username;
    this->password = password;
    
    state = Init;
    response.clear();
    messageAccepted = false;
    resultEmitted = false;
    
    if (socket) {
        socket->disconnectFromHost();
        socket->deleteLater();
    }
    
    socket = new QSslSocket(this);
    
    connect(socket, &QSslSocket::connected, this, &SmtpClient::onConnected);
    connect(socket, &QSslSocket::readyRead, this, &SmtpClient::onReadyRead);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QSslSocket::errorOccurred),
            this, &SmtpClient::onError);
    connect(socket, &QSslSocket::encrypted, this, [this]() {
        writeLog("🔒 Connexion TLS établie");
        sendCommand("EHLO localhost\r\n");
        response.clear();
    });
    connect(socket, &QSslSocket::sslErrors, this, [this](const QList<QSslError> &errors) {
        QStringList messages;
        for (const QSslError &error : errors) {
            messages << error.errorString();
        }
        writeLog("⚠️ SSL errors: " + messages.join(" | "));
        socket->ignoreSslErrors();
    });
    
    QString logMsg = QString("📧 Connexion au serveur SMTP: %1:%2").arg(smtpServer).arg(smtpPort);
    writeLog(logMsg);
    socket->connectToHost(smtpServer, smtpPort);
    
    // Timeout de connexion - 10 secondes max
    if (!socket->waitForConnected(10000)) {
        writeLog("❌ TIMEOUT: Impossible de se connecter au serveur SMTP en 10 secondes");
        emit emailSent(false, "Timeout de connexion au serveur SMTP. Vérifiez votre connexion Internet.");
        return;
    }
}

void SmtpClient::onConnected()
{
    writeLog("========================================");
    writeLog("✅ Connecté au serveur SMTP");
    writeLog("Serveur SMTP actif, en attente de la réponse 220...");
    writeLog("========================================");
    state = HandShake;
}

void SmtpClient::onReadyRead()
{
    QString responseLine = socket->readAll();
    writeLog(QString("📩 Réponse SMTP [STATE=%1]: %2").arg(state).arg(responseLine.trimmed()));
    response += responseLine;
    qDebug() << "========================================";
    qDebug() << "📩 Réponse SMTP [STATE=" << state << "]:";  
    qDebug() << responseLine.trimmed();
    qDebug() << "========================================";
    
    if (!responseLine.contains("\r\n"))
        return;
    
    switch (state) {
    case Init:
        state = HandShake;
        break;
        
    case HandShake:
        if (response.startsWith("220")) {
            sendCommand("EHLO localhost\r\n");
            state = Auth;
            response.clear();
        }
        break;
        
    case Auth:
        if (response.contains("250-STARTTLS") || response.contains("250 STARTTLS")) {
            sendCommand("STARTTLS\r\n");
            response.clear();
        } else if (response.startsWith("220")) {
            socket->startClientEncryption();
            response.clear();
        } else if (socket->isEncrypted() && response.contains("250")) {
            sendCommand("AUTH LOGIN\r\n");
            state = Pass;
            response.clear();
        }
        break;
        
    case User:
        if (response.contains("250")) {
            sendCommand("AUTH LOGIN\r\n");
            state = Pass;
            response.clear();
        }
        break;
        
    case Pass:
        if (response.startsWith("334")) {
            if (response.contains("VXNlcm5hbWU6") || !response.contains("UGFzc3dvcmQ6")) {
                // Demande username
                QString encodedUsername = username.toUtf8().toBase64();
                sendCommand(encodedUsername + "\r\n");
                response.clear();
            } else {
                // Demande password
                QString encodedPassword = password.toUtf8().toBase64();
                sendCommand(encodedPassword + "\r\n");
                state = Mail;
                response.clear();
            }
        }
        break;
        
    case Mail:
        if (response.startsWith("235")) {
            sendCommand("MAIL FROM:<" + from + ">\r\n");
            state = Rcpt;
            response.clear();
        } else if (response.startsWith("535")) {
            if (!resultEmitted) {
                resultEmitted = true;
                emit emailSent(false, "❌ Authentification échouée. Vérifiez vos identifiants.");
            }
            socket->close();
        }
        break;
        
    case Rcpt:
        if (response.startsWith("250")) {
            sendCommand("RCPT TO:<" + to + ">\r\n");
            state = Data;
            response.clear();
        }
        break;
        
    case Data:
        if (response.startsWith("250")) {
            sendCommand("DATA\r\n");
            state = Body;
            response.clear();
        }
        break;
        
    case Body:
        if (response.startsWith("354")) {
            QString message = "From: " + from + "\r\n";
            message += "To: " + to + "\r\n";
            message += "Subject: " + subject + "\r\n";
            message += "Date: " + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "\r\n";
            message += "MIME-Version: 1.0\r\n";
            message += "Content-Type: text/html; charset=UTF-8\r\n";
            message += "\r\n";
            message += body;
            message += "\r\n.\r\n";
            
            sendCommand(message);
            state = Quit;
            response.clear();
        }
        break;
        
    case Quit:
        if (response.startsWith("250")) {
            messageAccepted = true;
            sendCommand("QUIT\r\n");
            state = Close;
            response.clear();
        }
        break;
        
    case Close:
        if (response.startsWith("221") && !resultEmitted) {
            resultEmitted = true;
            emit emailSent(true, "✅ Email envoyé avec succès!");
        }
        socket->disconnectFromHost();
        break;
    }
}

void SmtpClient::onError(QAbstractSocket::SocketError error)
{
    QString errorMsg;

    if (error == QAbstractSocket::RemoteHostClosedError && messageAccepted) {
        writeLog("ℹ️ Connexion fermée par Gmail après envoi réussi (normal)");
        if (!resultEmitted) {
            resultEmitted = true;
            emit emailSent(true, "✅ Email envoyé avec succès!");
        }
        return;
    }
    
    writeLog("========================================");
    writeLog(QString("❌ ERREUR SMTP DÉTECTÉE! Code: %1").arg(error));
    
    switch (error) {
    case QAbstractSocket::HostNotFoundError:
        errorMsg = "❌ Serveur SMTP non trouvé (smtp.gmail.com). Vérifiez votre connexion Internet.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        errorMsg = "❌ Connexion refusée par le serveur SMTP. Le port 587 est peut-être bloqué.";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        errorMsg = "❌ Le serveur SMTP a fermé la connexion";
        break;
    case QAbstractSocket::SocketTimeoutError:
        errorMsg = "❌ Délai d'attente dépassé";
        break;
    default:
        errorMsg = "❌ Erreur SMTP: " + socket->errorString();
    }
    
    writeLog(errorMsg);
    writeLog(QString("Détails technique: %1").arg(socket->errorString()));
    writeLog("========================================");
    if (!resultEmitted) {
        resultEmitted = true;
        emit emailSent(false, errorMsg);
    }
}

void SmtpClient::sendCommand(const QString &command)
{
    qDebug() << "========================================";
    if (!command.trimmed().contains("LOGIN") && !command.contains("VGFz")) {
        qDebug() << "📤 Envoi commande SMTP:" << command.trimmed();
    } else {
        qDebug() << "📤 Envoi: [CREDENTIALS HIDDEN]";
    }
    qDebug() << "========================================";
    socket->write(command.toUtf8());
}
