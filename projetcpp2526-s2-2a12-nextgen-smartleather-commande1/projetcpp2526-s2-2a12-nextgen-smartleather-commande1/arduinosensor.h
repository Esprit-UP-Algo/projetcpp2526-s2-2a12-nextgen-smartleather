#ifndef ARDUINOSENSOR_H
#define ARDUINOSENSOR_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QString>
#include <QTimer>

class ArduinoSensor : public QObject
{
    Q_OBJECT

public:
    explicit ArduinoSensor(QObject *parent = nullptr);
    ~ArduinoSensor();

    // Connexion
    bool connectToPort(const QString &portName, qint32 baudRate = 9600);
    void disconnectFromPort();
    bool isConnected() const;

    // Lecture
    float getLastTemperature() const { return lastTemperature; }
    float getLastHumidity() const { return lastHumidity; }
    QString getLastError() const { return lastError; }

signals:
    // Signaux émis lors de la réception de données
    void temperatureReceived(float temperature, float humidity);
    void errorOccurred(const QString &error);
    void connectedStatusChanged(bool connected);

private slots:
    void handleReadyRead();
    void handlePortError(QSerialPort::SerialPortError error);

private:
    QSerialPort *serialPort;
    QTimer *reconnectTimer;
    QString lastError;
    float lastTemperature = 0.0f;
    float lastHumidity = 0.0f;
    QString dataBuffer;

    void parseArduinoData(const QString &data);
};

#endif // ARDUINOSENSOR_H
