#include "AsyncDeviceFinder.h"
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QVariant>
#include "SerialSettings.h"


int AsyncDeviceFinder::MAX_READLINE_ATTEMPT = SerialSettings::instance()->getAttemptsForReadLine();
static constexpr char PROPERTY_READ_ATTEMPT[] = "PROPERTY_READ_ATTEMPT";

AsyncDeviceFinder::AsyncDeviceFinder(IfceDeviceFactory *deviceFactory, QObject *parent)
    : IfceSerialFinder(parent), deviceFactory(deviceFactory)
{    
}

AsyncDeviceFinder::~AsyncDeviceFinder()
{
    for( auto serial : serials.values() )
        if( serial && serial->isOpen() )
            serial->close();
}

QList<QSharedPointer<IfceDevice>> AsyncDeviceFinder::discover()
{
    for( auto availableSerial : QSerialPortInfo::availablePorts() )
        prepare_serial(availableSerial);


    return QList<QSharedPointer<IfceDevice>>();
}

void AsyncDeviceFinder::onReadyRead()
{
    QPointer<QSerialPort> serial = qobject_cast<QSerialPort*>(sender());
    if( !serial && !serial->canReadLine() )
        return;
    const auto portName { serial->portName() };
    auto sharedSerial { serials.value(portName) };
    auto line { sharedSerial->readLine() };

    const auto serialInfo { availableSerials.value(portName) };

    QSharedPointer<IfceDevice> device;
    try {
        device = deviceFactory->create(line, serialInfo);
    } catch( ParsingException &e ) {
        emit signalSerialError(QString("%1: %2").arg(portName).arg(e.what()));
        action_parsing_error(sharedSerial);
        return;
    }

    emit signalDeviceFound(device);
    sharedSerial->close();
    sharedSerial.clear();
}

void AsyncDeviceFinder::onErrorOccurred(QSerialPort::SerialPortError error)
{
    if( error == QSerialPort::NoError )
        return;

    QPointer<QSerialPort> serial { qobject_cast<QSerialPort*>(sender()) };
    auto sharedSerial { serials.value(serial->portName()) };
    emit signalSerialError(QString("%1: %2").arg(sharedSerial->portName()).arg(sharedSerial->errorString()));

    if( sharedSerial->isOpen() )
        sharedSerial->close();
    sharedSerial.clear();     //delete later
}

void AsyncDeviceFinder::prepare_serial(const QSerialPortInfo &serialInfo)
{
    constexpr auto START_READ_LINE_ATTEMPT {0};

    auto serial { DeviceFinder::createSerial(serialInfo, true) };
    connect(serial.data(), &QSerialPort::readyRead, this, &AsyncDeviceFinder::onReadyRead);
    serial->setProperty(PROPERTY_READ_ATTEMPT, START_READ_LINE_ATTEMPT);
    serials.insert(serialInfo.portName(), serial);
    availableSerials.insert(serialInfo.portName(), serialInfo);

    serial->open(QIODevice::ReadOnly);
}

void AsyncDeviceFinder::action_parsing_error(QSharedPointer<QSerialPort> serial)
{
    auto attemptCount { serial->property(PROPERTY_READ_ATTEMPT).toInt() };
    if( ++attemptCount < MAX_READLINE_ATTEMPT ) {
        serial->setProperty(PROPERTY_READ_ATTEMPT, attemptCount);
        return;
    }

    serial->close();
    serial.clear();
}
