#ifndef JSONDEVICEFACTORY_H
#define JSONDEVICEFACTORY_H

#include "IfceDeviceFactory.h"
#include "JsonDevice.h"


/**
 * @brief The JsonDeviceFactory class creates JsonDevices that can
 * be utilized by {@link DeviceFinder}
 */
class JsonDeviceFactory : public IfceDeviceFactory
{
public:
    JsonDeviceFactory();
    ~JsonDeviceFactory();

    QSharedPointer<IfceDevice> create(const QByteArray &data, const QSerialPortInfo &portInfo) throw(ParsingException) override;
    QSharedPointer<IfceDevice> create(int id, const QSerialPortInfo &portInfo) override;
};

#endif // JSONDEVICEFACTORY_H
