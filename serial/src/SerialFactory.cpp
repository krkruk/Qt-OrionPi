#include "SerialFactory.h"
#include "DefaultSerialSettings.h"
#include <QSerialPort>
#include <QDebug>
SerialFactory::SerialFactory()
    : settings { QSharedPointer<DefaultSerialSettings>::create() }
{

}

SerialFactory::~SerialFactory()
{

}

QSharedPointer<QSerialPort> SerialFactory::create(const QSerialPortInfo &info, bool deleteItLater)
{
    QSharedPointer<QSerialPort> serial;
    if( deleteItLater )
        serial = QSharedPointer<QSerialPort>{ new QSerialPort(info), &QSerialPort::deleteLater };
    else
        serial = QSharedPointer<QSerialPort>::create(info);

    qDebug() << "AT port: " << serial->portName() << settings->getBaudrate();
    serial->setBaudRate( static_cast<QSerialPort::BaudRate>(settings->getBaudrate()),
                         QSerialPort::AllDirections);
    serial->setDataBits( static_cast<QSerialPort::DataBits>(settings->getDataBits()) );
    serial->setFlowControl( static_cast<QSerialPort::FlowControl>(settings->getFlowControl()) );
    serial->setParity( static_cast<QSerialPort::Parity>(settings->getParity()) );
    serial->setStopBits( static_cast<QSerialPort::StopBits>(settings->getStopBits()) );

    return serial;
}

void SerialFactory::setSerialSettings(QSharedPointer<IfceSerialSettings> settings)
{
    qDebug() << "Changing serial settings";
    this->settings = settings;
}
