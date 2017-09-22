#ifndef IFCESERIALPARSER_H
#define IFCESERIALPARSER_H

class ParsingException;


/**
 * @brief The IfceSerialParser interface provides basic interface for parsing
 * data in the system.
 *
 * Every single uC must implement ID parameter so then it can be recognized
 * as a device with a specified purpose. However, the interface does not
 * specify incoming data format. This a job of the derivatives.
 *
 * The derivatives of this interface can be therefore applied for device
 * discovering.
 */
class IfceSerialParser
{
public:
    virtual ~IfceSerialParser() {}

    /**
     * @brief parse Parses input data
     * Interface for parsing incoming data. The end-user has to implement it
     * in order to obtain "id" parameter from a serial device. The "id"
     * is an essential part for device recognition and has to be encoded
     * both on uC and a computer.
     * @param data Raw data received from a serial device
     * @return True if parsing is successful.
     * @throws ParsingExeption in an event of unsuccessful parsing.
     */
    virtual bool parse(const QByteArray &data) = 0;

    /**
     * @brief getId Returns an ID of the device
     * @return Integer
     */
    virtual int getId() const = 0;

    /**
     * @brief getValue Returns other values that might be critical
     * for parsing data. However, it is advised the proper parsing stage
     * is handled by the model itself.
     * @param valueId Enum of a value is to be returned.
     * @return QVariant
     */
    virtual QVariant getValue(int valueId) const = 0;
};

#endif // IFCESERIALPARSER_H
