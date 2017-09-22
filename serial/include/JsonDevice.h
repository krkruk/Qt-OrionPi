#ifndef JSONDEVICE_H
#define JSONDEVICE_H

#include <QObject>

#include "exceptions/ParsingException.h"
#include "IfceDevice.h"
#include "IfceDeviceFactory.h"
#include "JsonSerialParser.h"


/**
 * @brief The JsonDevice class parses incoming serial data from JSON format.
 *
 * The class allows parsing JSON data incoming from a serial device. See
 * {@link IfceDeviceFactory} and {@link DeviceFinder} for more details.
 * This class can be called directly, however for the most effictient serial
 * detection it should be wrapped with a factory and passed to {@link DeviceFinder}
 */
class JsonDevice : public IfceDevice
{
    QSerialPortInfo portInfo;
    JsonSerialParser parser;


public:
    JsonDevice(int id, QSharedPointer<IfceSerialSettings> settings, QSerialPortInfo portInfo = QSerialPortInfo());
    JsonDevice(QSharedPointer<IfceSerialSettings> settings, QSerialPortInfo portInfo = QSerialPortInfo());
    ~JsonDevice();

    void parse(const QByteArray &data) override;
    int getId() const override;
    QString getPortName() const override;
    QSerialPortInfo getSerialInfo() const override;    
};

#endif // JSONDEVICE_H
