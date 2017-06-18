#ifndef IFCESERIALFINDER_H
#define IFCESERIALFINDER_H
#include <QSharedPointer>
#include <QObject>
#include <QList>

class IfceDevice;
class IfceDeviceFactory;


/**
 * @brief The IfceSerialFinder interface allows implementing discovering of serial devices.
 */
class IfceSerialFinder : public QObject
{
    Q_OBJECT
public:
    IfceSerialFinder(QObject *parent = nullptr)
        : QObject(parent)
    {}
    virtual ~IfceSerialFinder(){}

    /**
     * @brief discover Discovers devices
     * @return List of discovered devices
     */
    virtual QList<QSharedPointer<IfceDevice>> discover() = 0;

signals:
    /**
     * @brief serialError The signal is emitted if connection to the device or data parsing error occures.
     * @param ttyError Descibres the reason of an error.
     */
    void signalSerialError(const QString &ttyError);
};
#endif // IFCESERIALFINDER_H
