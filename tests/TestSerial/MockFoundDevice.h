#ifndef MOCKFOUNDDEVICE_H
#define MOCKFOUNDDEVICE_H
#include "include/JsonDevice.h"


class MockFoundDevice : public JsonDevice
{
    QString portName;
public:
    MockFoundDevice(int id, const QString &portName)
        : JsonDevice(id), portName{ portName }
    {

    }
    ~MockFoundDevice(){}

    QString getPortName() const override
    {
        return portName;
    }
};

#endif // MOCKFOUNDDEVICE_H
