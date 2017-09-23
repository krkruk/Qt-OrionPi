#ifndef IFCESERIALOBSERVER_H
#define IFCESERIALOBSERVER_H

/**
 * @brief The IfceSerialObserver provides an interface for Observer pattern.
 *
 * The interface can be applied for any observer of the model.
 */
class IfceSerialObserver
{
public:
    virtual ~IfceSerialObserver() {}

    /**
     * @brief send Receives new data from model.
     * @param id ID of the model. The ID identifies the device the data is to be sent to.
     * @param data Raw data that is to be sent to the device.
     */
    virtual void send(int id, const QByteArray &data) = 0;
};

#endif // IFCESERIALOBSERVER_H
