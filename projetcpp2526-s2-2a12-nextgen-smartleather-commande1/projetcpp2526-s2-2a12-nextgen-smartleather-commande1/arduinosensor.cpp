#include "arduinosensor.h"
#include <QDebug>
#include <QRegularExpression>

ArduinoSensor::ArduinoSensor(QObject *parent)
    : QObject(parent), serialPort(nullptr), reconnectTimer(nullptr)
{
    serialPort = new QSerialPort(this);
    
    // Signaux de la communication série
    connect(serialPort, &QSerialPort::readyRead, this, &ArduinoSensor::handleReadyRead);
    connect(serialPort, QOverload<QSerialPort::SerialPortError>::of(&QSerialPort::errorOccurred),
            this, &ArduinoSensor::handlePortError);

    // Timer de reconnexion automatique
    reconnectTimer = new QTimer(this);
    reconnectTimer->setInterval(5000); // Essayer de reconnecter toutes les 5 secondes
}

ArduinoSensor::~ArduinoSensor()
{
    disconnectFromPort();
}

bool ArduinoSensor::connectToPort(const QString &portName, qint32 baudRate)
{
    if (serialPort->isOpen()) {
        disconnectFromPort();
    }

    serialPort->setPortName(portName);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort->open(QIODevice::ReadWrite)) {
        lastError = "";
        emit connectedStatusChanged(true);
        qDebug() << "Connected to" << portName << "at" << baudRate << "baud";
        return true;
    } else {
        lastError = QString("Impossible de se connecter à %1: %2").arg(portName, serialPort->errorString());
        emit errorOccurred(lastError);
        emit connectedStatusChanged(false);
        qDebug() << lastError;
        return false;
    }
}

void ArduinoSensor::disconnectFromPort()
{
    if (serialPort && serialPort->isOpen()) {
        serialPort->close();
        emit connectedStatusChanged(false);
        qDebug() << "Disconnected from Arduino";
    }
}

bool ArduinoSensor::isConnected() const
{
    return serialPort && serialPort->isOpen();
}

void ArduinoSensor::handleReadyRead()
{
    if (!serialPort)
        return;

    // Lire tous les données disponibles
    QByteArray data = serialPort->readAll();
    qDebug() << "📥 Données reçues du port:" << data.length() << "bytes -" << data;
    dataBuffer += QString::fromUtf8(data);

    // Chercher une ligne complète (terminée par \n)
    int newlineIndex = dataBuffer.indexOf('\n');
    while (newlineIndex != -1) {
        QString line = dataBuffer.left(newlineIndex).trimmed();
        dataBuffer.remove(0, newlineIndex + 1);

        if (!line.isEmpty()) {
            qDebug() << "📋 Parsing ligne:" << line;
            parseArduinoData(line);
        }

        newlineIndex = dataBuffer.indexOf('\n');
    }
}

void ArduinoSensor::parseArduinoData(const QString &data)
{
    // Formats supportés:
    // 1. "TEMP:25.5,HUM:60.2" (format optimisé)
    // 2. "Temperature: 25.5 C" (format texte)
    // 3. "ERROR: message" ou "Erreur"
    
    qDebug() << "Arduino data:" << data;

    // Vérifier les erreurs d'abord
    if (data.contains("ERROR", Qt::CaseInsensitive) || 
        data.contains("Erreur", Qt::CaseInsensitive)) {
        lastError = "Capteur DHT: " + data;
        emit errorOccurred(lastError);
        return;
    }

    bool tempFound = false;
    float temp = 0.0f;
    float hum = 0.0f;

    // Format 1: TEMP:25.5,HUM:60.2
    if (data.contains("TEMP:", Qt::CaseInsensitive)) {
        QRegularExpression tempRegex("(?:TEMP|T):([-\\d.]+)", QRegularExpression::CaseInsensitiveOption);
        QRegularExpression humRegex("(?:HUM|H):([-\\d.]+)", QRegularExpression::CaseInsensitiveOption);

        QRegularExpressionMatch tempMatch = tempRegex.match(data);
        QRegularExpressionMatch humMatch = humRegex.match(data);

        if (tempMatch.hasMatch()) {
            temp = tempMatch.captured(1).toFloat();
            hum = humMatch.hasMatch() ? humMatch.captured(1).toFloat() : 0.0f;
            tempFound = true;
        }
    }
    // Format 2: "Temperature: 25.5 C" ou "Humidity: 60.2 %"
    else if (data.contains("Temperature", Qt::CaseInsensitive)) {
        QRegularExpression tempRegex("Temperature\\s*:\\s*([-\\d.]+)");
        QRegularExpressionMatch tempMatch = tempRegex.match(data);

        if (tempMatch.hasMatch()) {
            temp = tempMatch.captured(1).toFloat();
            hum = lastHumidity; // Garde la dernière valeur d'humidité
            tempFound = true;
        }
    }

    if (tempFound) {
        lastTemperature = temp;
        lastHumidity = hum;
        emit temperatureReceived(lastTemperature, lastHumidity);
        lastError = "";
        qDebug() << "Parsed - Temp:" << lastTemperature << "°C, Humidity:" << lastHumidity << "%";
    } else {
        lastError = "Format de données non reconnu: " + data;
        emit errorOccurred(lastError);
    }
}

void ArduinoSensor::handlePortError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::NoError)
        return;

    lastError = QString("Erreur port série: %1").arg(serialPort->errorString());
    emit errorOccurred(lastError);
    emit connectedStatusChanged(false);
    qDebug() << lastError;
}
