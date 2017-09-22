#include "JsonDevice.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>

#include "IfceSerialSettings.h"

JsonDevice::JsonDevice(int id, QSharedPointer<IfceSerialSettings> settings, QSerialPortInfo portInfo)
    : portInfo{ portInfo },
      parser(settings->getId(), id)
{
}

JsonDevice::JsonDevice(QSharedPointer<IfceSerialSettings> settings, QSerialPortInfo portInfo)
    : portInfo { portInfo }, parser(settings->getId())
{
}

JsonDevice::~JsonDevice() {}

void JsonDevice::parse(const QByteArray &data)
{
    try {
        parser.parse(data);
    } catch(...) {
        throw;
    }
}

int JsonDevice::getId() const
{
    return parser.getId();
}

QString JsonDevice::getPortName() const
{
    return portInfo.portName();
}

QSerialPortInfo JsonDevice::getSerialInfo() const
{
    return portInfo;
}
