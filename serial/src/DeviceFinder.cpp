#include "DeviceFinder.h"
#include "include/JsonDevice.h"
#include "SerialSettings.h"
#include <QDebug>

const char DeviceFinder::READ_LINE_ERR_MSG[] = "Cannot read a line";

DeviceFinder::DeviceFinder(IfceDeviceFactory *factory, QObject *parent)
    : IfceSerialFinder(parent), factory{factory}
{
}

DeviceFinder::~DeviceFinder()
{
}

QList<QSharedPointer<IfceDevice> > DeviceFinder::discover()
{
    QList<QSharedPointer<IfceDevice> > devices;

    for( const auto &serialInfo : QSerialPortInfo::availablePorts() )
        if( auto &&device = run_serial(serialInfo) )
            devices.append(device);

    return devices;
}

QSharedPointer<QSerialPort> DeviceFinder::createSerial(const QSerialPortInfo &serialInfo, bool deleteItLater)
{
    QSharedPointer<QSerialPort> serial;
    if( deleteItLater )
        serial = QSharedPointer<QSerialPort>{ new QSerialPort(serialInfo), &QSerialPort::deleteLater };
    else
        serial = QSharedPointer<QSerialPort>::create(serialInfo);

    auto *s { SerialSettings::instance() };

    serial->setBaudRate( static_cast<QSerialPort::BaudRate>(s->getBaudrate()),
                         QSerialPort::AllDirections);
    serial->setDataBits( static_cast<QSerialPort::DataBits>(s->getDataBits()) );
    serial->setFlowControl( static_cast<QSerialPort::FlowControl>(s->getFlowControl()) );
    serial->setParity( static_cast<QSerialPort::Parity>(s->getParity()) );
    serial->setStopBits( static_cast<QSerialPort::StopBits>(s->getStopBits()) );

    return serial;
}

QSharedPointer<IfceDevice> DeviceFinder::run_serial(const QSerialPortInfo &serialInfo)
{
    QSharedPointer<QSerialPort> serial { createSerial(serialInfo) };

    if( !open_serial(serial) )
        return QSharedPointer<IfceDevice>();

    if( !wait_for_connection(serial) )
        return handle_error_msg(serial, serial->errorString());


    const auto max_attempts { SerialSettings::instance()->getAttemptsForReadLine() };
    auto attempt {0};
    do {
        if( !wait_for_read_line(serial) )
            return handle_error_msg(serial, READ_LINE_ERR_MSG);

        auto line { serial->readLine() };
        try {
            return (QSharedPointer<IfceDevice>{
                        factory->create(line, serialInfo) });
        } catch(ParsingException &e) {
            emit signalSerialError(QString("%1: %2").arg(serialInfo.portName()).arg(e.what()));
        }
    } while( ++attempt < max_attempts );

    return QSharedPointer<IfceDevice>();
}

bool DeviceFinder::open_serial(QSharedPointer<QSerialPort> &serial)
{
    if( serial->open(QIODevice::ReadOnly) )
        return true;

    emit signalSerialError(QString("%1: %2")
                     .arg(serial->portName())
                     .arg(serial->errorString()));
    return false;
}

bool DeviceFinder::wait_for_connection(QSharedPointer<QSerialPort> &serial)
{
    const auto waitForReadLine{ SerialSettings::instance()->getWaitForReadLineMs() };
    return serial->waitForReadyRead(waitForReadLine);
}

bool DeviceFinder::wait_for_read_line(QSharedPointer<QSerialPort> &serial)
{
    const auto waitForReadLine{ SerialSettings::instance()->getWaitForReadLineMs() };
    const auto MAX_READ_ATTEMPT { SerialSettings::instance()->getAttemptsForReadLine() };
    auto attempt {0};

    while( !serial->canReadLine() ) {
        serial->waitForReadyRead(waitForReadLine);
        if( ++attempt >= MAX_READ_ATTEMPT )
            return false;
    }

    return true;
}

QSharedPointer<IfceDevice> DeviceFinder::handle_error_msg(QSharedPointer<QSerialPort> &serial, const QString &msg)
{
    emit signalSerialError(QString("%1: %2")
                     .arg(serial->portName())
                     .arg(msg));
    return QSharedPointer<IfceDevice>();
}
