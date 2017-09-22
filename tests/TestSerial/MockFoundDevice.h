#ifndef MOCKFOUNDDEVICE_H
#define MOCKFOUNDDEVICE_H
#include "include/JsonDevice.h"
#include "include/DefaultSerialSettings.h"

class MockFoundDevice : public JsonDevice
{
    QString portName;
public:
    MockFoundDevice(int id, const QString &portName, QSharedPointer<DefaultSerialSettings> settings)
        : JsonDevice(id, settings), portName{ portName }
    {

    }
    ~MockFoundDevice(){}

    QString getPortName() const override
    {
        return portName;
    }
};

#endif // MOCKFOUNDDEVICE_H
