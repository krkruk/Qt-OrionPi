#ifndef IFCESERIALMODEL_H
#define IFCESERIALMODEL_H
#include <QWeakPointer>
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
    virtual void updateModel(const QByteArray &line) = 0;

    /**
     * @brief addObserver Adds observer (View)
     * @param observer
     */
    virtual void addObserver(QWeakPointer<IfceSerialObserver> observer) = 0;

    /**
     * @brief delObserver Deletes observer
     * @param observer
     * @return
     */
    virtual bool delObserver(QWeakPointer<IfceSerialObserver> observer) = 0;

    /**
     * @brief notifyObservers Notifies an observer that internal data of a model
     * has changed and a serial device has to be updated.
     */
    virtual void notifyObservers() = 0;
};

#endif // IFCESERIALMODEL_H
