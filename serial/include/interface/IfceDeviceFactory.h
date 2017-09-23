#ifndef IFCEDEVICEFACTORY_H
#define IFCEDEVICEFACTORY_H

#include "exceptions/ParsingException.h"
#include <QSharedPointer>

class IfceDevice;
class QSerialPortInfo;
class IfceSerialSettings;


/**
 * @brief The IfceDeviceFactory interface provides methods for creation of
 */
class IfceDeviceFactory
{
public:
    virtual ~IfceDeviceFactory() {}
    virtual QSharedPointer<IfceDevice> create(const QByteArray &data, const QSerialPortInfo &portInfo, QWeakPointer<IfceSerialSettings> settings) = 0;
    virtual QSharedPointer<IfceDevice> create(int id, const QSerialPortInfo &portInfo, QWeakPointer<IfceSerialSettings> settings) = 0;
};


#endif // IFCEDEVICEFACTORY_H
