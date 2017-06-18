#ifndef IFCESERIALMODEL_H
#define IFCESERIALMODEL_H

class QByteArray;
class IfceSerialObserver;


/**
 * @brief The IfceSerialModel is an interface for deriving models for serial devices.
 *
 * IfceSerialModel provides interfaces for the following tasks:
 * * parsing incoming data from the controller and thus the serial server
 * * (un)registering observers
 * * notifying observers
 *
 * Moreover IfceSerialModel provides a getter for ID parameter the model is
 * described with. The value is essential for recognition of the serial
 * device and its function in the system.
 */
class IfceSerialModel
{
    int id;
public:
    explicit IfceSerialModel(int id)
        : id{id}{}

    virtual ~IfceSerialModel(){}

    int getId() const { return id; }

    /**
     * @brief update Receives data from the controller.
     * @param line Raw data received from the serial device.
     */
    virtual void update(const QByteArray &line) = 0;

    virtual void addObserver(IfceSerialObserver *observer) = 0;
    virtual bool delObserver(IfceSerialObserver *observer) = 0;
    virtual void notifyObservers() = 0;
};

#endif // IFCESERIALMODEL_H
