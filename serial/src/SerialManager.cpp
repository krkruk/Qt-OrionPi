#include "SerialManager.h"
#include "IfceDevice.h"
#include "DeviceFinder.h"
#include <SerialConstants.h>
#include <QDebug>


SerialManager::SerialManager(QObject *parent)
    : QObject(parent)
{
}

SerialManager::~SerialManager()
{
    auto serials { serialsByPortName.values() };
    for(auto &serial : serials) {
        if( serial->isOpen() )
            serial->close();
    }

}

void SerialManager::setDevices(const QList<QSharedPointer<IfceDevice> > &devices)
{
    for( const auto &device : devices )
    {
        const auto id { device->getId() };
        const auto portName { device->getPortName() };
        auto serial { DeviceFinder::createSerial(device->getSerialInfo(), true) };
        portNamesById[id] = portName;
        serialsByPortName[portName] = serial;
    }
}

bool SerialManager::hasSerial(int id) const
{
    const auto portName = portNamesById.value(id, "");
    if( portName.isEmpty() )
        return false;

    const auto serial { serialsByPortName.value(portName) };

    return !serial.isNull();
}

void SerialManager::setController(IfceSerialController *controller)
{
    this->controller = controller;
}

QByteArray SerialManager::getLastSent(int id) const
{
    return lastSent[id];
}

QList<int> SerialManager::start()
{
    QList<int> serialFailedToStart;

    for( const auto &portName : serialsByPortName.keys() )
    {
        const auto id { get_id_based_on_port_name(portName) };
        if( id == -1 ) break;
        auto serial { serialsByPortName[portName] };
        if( !setup_port(serial) )
            serialFailedToStart.append(id);
    }

    return serialFailedToStart;
}

void SerialManager::onLineReceived(int id, const QByteArray &data)
{
    if( controller )
        controller->onLineReceived(id, data);
}

void SerialManager::send(int id, const QByteArray &data)
{
    const auto portName { portNamesById.value(id) };
    if( portName.isEmpty() ) return;
    auto serial { serialsByPortName.value(portName) };
    if( serial.isNull() ) return;
    qDebug() << "Data sent" << data;
    serial->write(data + CONST::SETTINGS::SERIAL::END_LINE);
    lastSent[id] = data;
}

void SerialManager::onErrorOccured(QSerialPort::SerialPortError error)
{
    if( error == QSerialPort::NoError )
        return;

    auto *serialPtr { qobject_cast<QSerialPort *>( sender() ) };
    if( !serialPtr ) return;

    emit serialError(get_id_based_on_port_name(serialPtr->portName()),
                     serialPtr->errorString());

    auto serial { serialsByPortName.take(serialPtr->portName()) };
    if( serial->isOpen() )
        serial->close();

    serial.clear();
}

bool SerialManager::setup_port(QSharedPointer<QSerialPort> serial)
{
    if( !serial->open(QSerialPort::ReadWrite) )
        return false;

    connections(serial);

    return true;
}

void SerialManager::connections(QSharedPointer<QSerialPort> serial)
{
    connect(serial.data(), &QSerialPort::readyRead, this, [&]{
        QSerialPort *device = qobject_cast<QSerialPort*>( sender() );
        if( !device ) return;
        const auto portName { device->portName() };
        const auto id { get_id_based_on_port_name(portName) };
        if( device->canReadLine() ) onLineReceived(id, device->readLine());
    });

    connect(serial.data(), &QSerialPort::errorOccurred,
            this, &SerialManager::onErrorOccured);
}

int SerialManager::get_id_based_on_port_name(const QString &portName) const
{
    return portNamesById.key(portName, -1);
}
