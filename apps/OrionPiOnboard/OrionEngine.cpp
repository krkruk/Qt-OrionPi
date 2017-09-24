#include "OrionEngine.h"
#include <string>
#include <QTimer>
#include <QTime>
#include "settings/SerialSettings.h"
#include "Orion/Drive/JsonChassisFeedbackGenerator.h"
#include <TcpServer.h>

namespace {
constexpr auto MAX_PENDING_CONN { 1 };
constexpr auto LISTEN_PORT { 5000 };
constexpr auto ID_FRONT_LEFT_WHEEL { 0 };
constexpr auto ID_FRONT_RIGHT_WHEEL { 1 };
constexpr auto ID_REAR_LEFT_WHEEL { 2 };
constexpr auto ID_REAR_RIGHT_WHEEL { 3 };

}


OrionEngine::OrionEngine(QObject *parent)
    : QObject(parent),
      serialManager( new SerialManager(this) ),
      serialController( QSharedPointer<SerialController>::create() ),
      chassisModel ( new Orion::ChassisModel(this) ),
      server( new TcpServer(this) ),
      sendFeedbackTimer( new QTimer(this) )
{
    connections();
    setup_serial();
    setup_server();
    sendFeedbackTimer->setInterval(100);
    sendFeedbackTimer->start();
}

OrionEngine::~OrionEngine()
{
}

void OrionEngine::onRemoteMessageReceived(const QByteArray &message)
{
    if( !chassisModel )
        return;

    chassisModel->updateState(QByteArray(message.toStdString().c_str()));
    chassisModel->notifyAll();
}

void OrionEngine::onFeedbackTimerTimeout()
{
    if( !server )
        return;

    auto feedbackData = chassisModel->getFeedbackData();
    server->send(feedbackData.toStdString() + "\r\n");
}

void OrionEngine::connections()
{
    connect(server.data(), &IfceServer::signalMessageReceived, this, &OrionEngine::onRemoteMessageReceived);
    connect(serialManager.data(), &SerialManager::serialError, this,
            [&](int id, const QString &errorString){
        qDebug() << QString("Error occured! DeviceId: %1 - error: %2").arg(id).arg(errorString);
    });

    connect(sendFeedbackTimer.data(), &QTimer::timeout, this, &OrionEngine::onFeedbackTimerTimeout);
}

void OrionEngine::setup_serial()
{
    QScopedPointer<DeviceFinder> finder(new ThreadedDeviceFinder( new JsonDeviceFactory, this));
    finder->setSerialSettings(QSharedPointer<SerialSettings>::create());
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

    chassisModel->setDriveAlgorithm(QSharedPointer<Orion::JsonDriveModeDirect>::create());
    chassisModel->setFeedbackGeneratorAlgorithm(QSharedPointer<Orion::JsonChassisFeedbackGenerator>::create());
    chassisModel->addWheel(frontLeftWheel);
    chassisModel->addWheel(frontRightWheel);
    chassisModel->addWheel(rearLeftWheel);
    chassisModel->addWheel(rearRightWheel);

    serialController->addModel(frontLeftWheel);
    serialController->addModel(frontRightWheel);
    serialController->addModel(rearLeftWheel);
    serialController->addModel(rearRightWheel);

    serialManager->setController(serialController);
    frontLeftWheel->addObserver(serialManager);
    frontRightWheel->addObserver(serialManager);
    rearLeftWheel->addObserver(serialManager);
    rearRightWheel->addObserver(serialManager);

    serialManager->setDevices(devices);
    auto notConnected { serialManager->start() };
    echo_not_connected_devices(notConnected);
}

void OrionEngine::setup_server()
{
    server->listen(QHostAddress::Any, LISTEN_PORT);
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
