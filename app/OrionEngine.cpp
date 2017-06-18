#include "OrionEngine.h"
#include <QTime>


static constexpr auto MAX_PENDING_CONN { 1 };
static constexpr auto LISTEN_PORT { 5000 };
static constexpr auto ID_FRONT_LEFT_WHEEL { 0 };
static constexpr auto ID_FRONT_RIGHT_WHEEL { 1 };
static constexpr auto ID_REAR_LEFT_WHEEL { 2 };
static constexpr auto ID_REAR_RIGHT_WHEEL { 3 };


OrionEngine::OrionEngine(QObject *parent)
    : QObject(parent),
      serialManager( new SerialManager(this) ),
      serialController( new SerialController ),
      chassisModel ( new Orion::ChassisModel(this) ),
      server( new QTcpServer(this) )
{
    connections();
    setup_serial();
    setup_server();
}

OrionEngine::~OrionEngine()
{
}

void OrionEngine::onNewConnection()
{
    socket = server->nextPendingConnection();
    server->close();
    qDebug() << "Socket connected at port:" << LISTEN_PORT;
    connect(socket.data(), &QTcpSocket::readyRead, this, &OrionEngine::onSocketReadyRead);
    connect(socket.data(), static_cast< void(QTcpSocket::*)(QAbstractSocket::SocketError) >(&QTcpSocket::error),
            this, &OrionEngine::onSocketError);
    connect(socket.data(), &QTcpSocket::disconnected, this, &OrionEngine::onSocketDisconnected);
}

void OrionEngine::onServerError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Error code:" << static_cast<int>(socketError);
    qDebug() << "Error text: " << server->errorString();
    server.reset();
}

void OrionEngine::onSocketReadyRead()
{
    if( socket->canReadLine() ) {
        const auto line { socket->readLine() };
        qDebug() << "Data received:" << line;
        chassisModel->onRemoteDataReceived(line);
        chassisModel->notifyAll();
    }
}

void OrionEngine::onSocketError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Error code:" << static_cast<int>(socketError);
    qDebug() << "Error text: " << socket->errorString();
    if( socket->isOpen() )
        socket->close();

    socket->deleteLater();
}

void OrionEngine::onSocketDisconnected()
{
    qDebug() << "TCP client disconnected";
    emit aboutToClose();
}

void OrionEngine::connections()
{
    connect(server.data(), &QTcpServer::newConnection, this, &OrionEngine::onNewConnection);
    connect(server.data(), &QTcpServer::acceptError, this, &OrionEngine::onServerError);
    connect(serialManager.data(), &SerialManager::serialError, this,
            [&](int id, const QString &errorString){
        qDebug() << QString("Error occured! DeviceId: %1 - error: %2").arg(id).arg(errorString);
    });
}

void OrionEngine::setup_serial()
{
    QScopedPointer<DeviceFinder> finder(new ThreadedDeviceFinder( new JsonDeviceFactory, this));
    QTime t1 = QTime::currentTime();
    t1.start();
    qDebug() << "Starting discovery of serial devices...";
    auto devices { finder->discover() };
    echo_found_serials(devices);
    qDebug() << "Elapsed milliseconds for search: " << t1.elapsed();


    auto frontLeftWheel { QSharedPointer<Orion::WheelModel>::create(ID_FRONT_LEFT_WHEEL) };
    auto frontRightWheel { QSharedPointer<Orion::WheelModel>::create(ID_FRONT_RIGHT_WHEEL) };
    auto rearLeftWheel { QSharedPointer<Orion::WheelModel>::create(ID_REAR_LEFT_WHEEL) };
    auto rearRightWheel { QSharedPointer<Orion::WheelModel>::create(ID_REAR_RIGHT_WHEEL) };

    chassisModel->setDriveAlgorithm(QSharedPointer<Orion::DriveModeDirect>::create());
    chassisModel->addWheel(frontLeftWheel);
    chassisModel->addWheel(frontRightWheel);
    chassisModel->addWheel(rearLeftWheel);
    chassisModel->addWheel(rearRightWheel);

    serialController->addModel(frontLeftWheel);
    serialController->addModel(frontRightWheel);
    serialController->addModel(rearLeftWheel);
    serialController->addModel(rearRightWheel);

    serialManager->setController(serialController.data());
    frontLeftWheel->addObserver(serialManager.data());
    frontRightWheel->addObserver(serialManager.data());
    rearLeftWheel->addObserver(serialManager.data());
    rearRightWheel->addObserver(serialManager.data());

    serialManager->setDevices(devices);
    auto notConnected { serialManager->start() };
    echo_not_connected_devices(notConnected);
}

void OrionEngine::setup_server()
{
    server->setMaxPendingConnections(MAX_PENDING_CONN);
    server->listen(QHostAddress::LocalHost, LISTEN_PORT);
}

void OrionEngine::echo_found_serials(const QList<QSharedPointer<IfceDevice> > &foundDevices)
{
    for( const auto &device : foundDevices )
        qDebug() << QString("Found device ID: %1 @ %2")
                    .arg(device->getId())
                    .arg(device->getPortName());
}

void OrionEngine::echo_not_connected_devices(const QList<int> &notConnectedIds)
{
    for( auto id : notConnectedIds )
        qDebug() << "Could not connect the device with ID:" << id;
}
