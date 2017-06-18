#include "JsonDeviceFactory.h"
#include <QSharedPointer>

JsonDeviceFactory::JsonDeviceFactory()
{

}

JsonDeviceFactory::~JsonDeviceFactory()
{

}

QSharedPointer<IfceDevice> JsonDeviceFactory::create(const QByteArray &data, const QSerialPortInfo &portInfo) throw(ParsingException)
{
    try {
        return QSharedPointer<IfceDevice>{new JsonDevice{data, portInfo} };
    } catch(...) {
        throw;
    }
}

QSharedPointer<IfceDevice> JsonDeviceFactory::create(int id, const QSerialPortInfo &portInfo)
{
    return QSharedPointer<IfceDevice>{ new JsonDevice{id, portInfo} };
}
