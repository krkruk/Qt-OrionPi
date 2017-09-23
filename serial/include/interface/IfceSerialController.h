#ifndef IFCESERIALCONTROLLER_H
#define IFCESERIALCONTROLLER_H
#include <QSharedPointer>

class IfceSerialModel;
class QByteArray;

/**
 * @brief The IfceSerialController interface allows communication between view and models.
 *
 * Any class derived from the interface should implement single way data exchange between
 * a view (a serial device server) and a model (a data representation).
 */
class IfceSerialController
{
public:
    virtual ~IfceSerialController() {}

    /**
     * @brief addModel Adds model that should be informed about incoming data from the serial server.
     * @param model {@link IfceSerialModel}
     */
    virtual void addModel(QSharedPointer<IfceSerialModel> model) = 0;

    /**
     * @brief onLineReceived Strategy method that allows forwarding data to the particular model object
     * described by id.
     * @param id ID of the model the data is addressed to
     * @param line Data to be parsed by the model.
     */
    virtual void onLineReceived(int id, const QByteArray &line) = 0;

    /**
     * @brief getLastReceived Returns the data that was recently received from the view
     * @param id ID of serial device that recently has sent data.
     * @return Raw data.
     */
    virtual QByteArray getLastReceived(int id) const = 0;
};

#endif // IFCESERIALCONTROLLER_H
