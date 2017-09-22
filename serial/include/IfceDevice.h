#ifndef IFCEDEVICE_H
#define IFCEDEVICE_H
#include <QSerialPortInfo>

#include "exceptions/ParsingException.h"


/**
 * @brief The IfceDevice class allows detecting serial devices based on hard-coded ID.
 *
 * The class is essential in assigning dynamically serial device to an
 * appropriate model. Every data message at discovering must contain ID. Based on ID
 * value the serial can be therefore assigned to the model through MVC pattern.
 *
 * The class connects to the serial device and attempts to read incoming data
 * according to the parameters (see {@link SerialSettings}).
 */
class IfceDevice
{
public:
    virtual ~IfceDevice() {}

    /**
     * @brief parse Parser incoming data.
     * @param data Raw data.
     */
    virtual void parse(const QByteArray &data) = 0;
    virtual int getId() const = 0;
    virtual QString getPortName() const = 0;
    virtual QSerialPortInfo getSerialInfo() const = 0;
};

#endif // IFCEDEVICE_H
