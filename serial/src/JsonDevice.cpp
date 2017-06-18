#include "JsonDevice.h"
#include "SerialSettings.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>


JsonDevice::JsonDevice(int id, QSerialPortInfo portInfo)
    : portInfo{ portInfo }, parser(id)
{
}

JsonDevice::JsonDevice(const QByteArray &lineData, const QSerialPortInfo &portInfo) throw(ParsingException)
    : portInfo{ portInfo }
{
    try {
        parse(lineData);
    } catch(...) {
        throw;
    }
}

JsonDevice::~JsonDevice() {}

void JsonDevice::parse(const QByteArray &data) throw(ParsingException)
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
