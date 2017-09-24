#ifndef ORIONENGINE_H
#define ORIONENGINE_H
#include <memory>

#include <QObject>
#include <QDebug>
#include <QScopedPointer>
#include <QScopedPointerObjectDeleteLater>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>

#include "include/DeviceFinder.h"
#include "include/ThreadedDeviceFinder.h"
#include "include/JsonDeviceFactory.h"
#include "include/SerialManager.h"
#include "include/SerialController.h"
#include "Orion/Drive/WheelModel.h"
#include "Orion/Drive/JsonDriveModeDirect.h"
#include "Orion/Drive/ChassisModel.h"

class QTimer;


class OrionEngine : public QObject
{
    Q_OBJECT
public:
    explicit OrionEngine(QObject *parent = 0);
    ~OrionEngine();

signals:
    void aboutToClose();


private slots:
    void onNewConnection();
    void onServerError(QAbstractSocket::SocketError socketError);
    void onSocketReadyRead();
    void onSocketError(QAbstractSocket::SocketError socketError);
    void onSocketDisconnected();
    void onFeedbackTimerTimeout();

private:
    void connections();
    void setup_serial();
    void setup_server();
    void echo_found_serials(const QList<QSharedPointer<IfceDevice> > &foundDevices);
    void echo_not_connected_devices(const QList<int> &notConnectedIds);

    QSharedPointer<SerialManager> serialManager;
    QSharedPointer<SerialController> serialController;
    QScopedPointer<Orion::ChassisModel> chassisModel;

    QScopedPointer<QTcpServer, QScopedPointerObjectDeleteLater<QTcpServer>> server;
    QPointer<QTcpSocket> socket;
    QScopedPointer<QTimer> sendFeedbackTimer;
};

#endif // ORIONENGINE_H
