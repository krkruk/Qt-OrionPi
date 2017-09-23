#include "JsonDeviceFactory.h"
#include <QSharedPointer>
#include "interface/IfceSerialSettings.h"

JsonDeviceFactory::JsonDeviceFactory()
{

}

JsonDeviceFactory::~JsonDeviceFactory()
{

}

QSharedPointer<IfceDevice> JsonDeviceFactory::create(int id, const QSerialPortInfo &portInfo, QWeakPointer<IfceSerialSettings> settings)
{
    return QSharedPointer<IfceDevice>{ new JsonDevice{id, settings.toStrongRef(), portInfo} };
}

QSharedPointer<IfceDevice> JsonDeviceFactory::create(const QByteArray &data, const QSerialPortInfo &portInfo, QWeakPointer<IfceSerialSettings> settings)
{
    auto device = QSharedPointer<JsonDevice>::create(settings.toStrongRef(), portInfo);
    device->parse(data);
    return device;
}
