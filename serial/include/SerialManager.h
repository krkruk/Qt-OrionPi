#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H
#include <QObject>
#include <QSharedPointer>
#include <QPointer>
#include <QHash>
#include <QSerialPort>


#include "serial_global.h"
#include "IfceSerialFinder.h"
#include "IfceSerialObserver.h"
#include "SerialController.h"

class SerialController;
class SERIALSHARED_EXPORT SerialManager : public QObject, public IfceSerialObserver
{
    Q_OBJECT
    friend class SerialManagerPrivate;
public:
    SerialManager(QObject *parent = nullptr);
    ~SerialManager();

    void setDevices(const QList<QSharedPointer<IfceDevice>> &devices);
    bool hasSerial(int id) const;

    void setController(IfceSerialController *controller);

    QByteArray getLastSent(int id) const;

    QList<int> start();

signals:
    void serialError(int id, const QString &errorString);


public slots:
    void onLineReceived(int id, const QByteArray &data);
    void send(int id, const QByteArray &data) override;


private slots:
    void onErrorOccured(QSerialPort::SerialPortError error);


private:
    QHash<int, QString> portNamesById;
    QHash<QString, QSharedPointer<QSerialPort>> serialsByPortName;
    QHash<int, QByteArray> lastSent;
    IfceSerialController *controller;

    bool setup_port(QSharedPointer<QSerialPort> serial);
    void connections(QSharedPointer<QSerialPort> serial);

    int get_id_based_on_port_name(const QString &portName) const;
};

#endif // SERIALMANAGER_H
