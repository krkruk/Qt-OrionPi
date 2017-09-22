#include "DeviceFinder.h"
#include "include/JsonDevice.h"
#include "DefaultSerialSettings.h"
#include "SerialFactory.h"
#include <QDebug>

const char DeviceFinder::READ_LINE_ERR_MSG[] = "Cannot read a line";

DeviceFinder::DeviceFinder(IfceDeviceFactory *factory, QObject *parent)
    : IfceSerialFinder(parent),
      factory{factory},
      settings{QSharedPointer<DefaultSerialSettings>::create()}
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

void DeviceFinder::setSerialSettings(QSharedPointer<IfceSerialSettings> settings)
{
    this->settings = settings;
}

QSharedPointer<IfceDevice> DeviceFinder::run_serial(const QSerialPortInfo &serialInfo)
{
    SerialFactory serialFactory;
    serialFactory.setSerialSettings(settings);
    QSharedPointer<QSerialPort> serial { serialFactory.create(serialInfo) };

    if( !open_serial(serial) )
        return QSharedPointer<IfceDevice>();

    if( !wait_for_connection(serial) )
        return handle_error_msg(serial, serial->errorString());


    const auto max_attempts { settings->getAttemptsForReadLine() };
    auto attempt {0};
    do {
        if( !wait_for_read_line(serial) )
            return handle_error_msg(serial, READ_LINE_ERR_MSG);

        auto line { serial->readLine() };
        try {
            return (QSharedPointer<IfceDevice>{
                        factory->create(line, serialInfo, settings) });
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
    const auto waitForReadLine{ settings->getWaitForReadLineMs() };
    return serial->waitForReadyRead(waitForReadLine);
}

bool DeviceFinder::wait_for_read_line(QSharedPointer<QSerialPort> &serial)
{
    const auto waitForReadLine{ settings->getWaitForReadLineMs() };
    const auto MAX_READ_ATTEMPT { settings->getAttemptsForReadLine() };
    auto attempt {0};

    while( serial->waitForReadyRead(waitForReadLine) && !serial->canReadLine() ) {
        if( ++attempt >= MAX_READ_ATTEMPT )
            return false;
    }

//    while( ++attempt >= MAX_READ_ATTEMPT ) {
//        if ( serial->waitForReadyRead() && serial->canReadLine() ) {
//            qDebug() << "LINE:" << serial->readLine();
//            return true;
//        }
//    }

    return true;
//    return false;
}

QSharedPointer<IfceDevice> DeviceFinder::handle_error_msg(QSharedPointer<QSerialPort> &serial, const QString &msg)
{
    emit signalSerialError(QString("%1: %2")
                     .arg(serial->portName())
                     .arg(msg));
    return QSharedPointer<IfceDevice>();
}

