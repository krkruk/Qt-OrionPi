#ifndef SERIALFACTORY_H
#define SERIALFACTORY_H
#include <QSharedPointer>

class IfceSerialSettings;
class QSerialPort;
class QSerialPortInfo;

class SerialFactory
{
public:
    SerialFactory();
    ~SerialFactory();

    /**
     * @brief createCreates an instance of QSerialPort.
     * Creates an instance of QSerialPort based on read {@link SerialSettings} values
     * @param info Information about serial port the method creates an instance of QSerialPort
     * @param deleteItLater Creates an object with a custom deleter QSerialPort::deleteLater();
     * @return Instance of {@link QSerialPort}
     */
    QSharedPointer<QSerialPort> create(const QSerialPortInfo &info, bool deleteItLater = false);
    void setSerialSettings(QSharedPointer<IfceSerialSettings> settings);

private:
    QSharedPointer<IfceSerialSettings> settings;
};

#endif // SERIALFACTORY_H
