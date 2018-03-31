#include "OrionEngine.h"
#include <string>
#include <QCoreApplication>
#include <QSettings>
#include <QTimer>
#include <QTime>
#include <QFile>
#include "settings/SerialSettings.h"
#include "Orion/Drive/JsonChassisFeedbackGenerator.h"
#include "Orion/Drive/ProtobufDriveModeDirect.h"
#include "Orion/Drive/ProtobufChassisFeedbackGenerator.h"
#include "inputs/TcpInputSource.h"
#include "inputs/GamepadInputSource.h"

#include "protocolEnums.pb.h"
#include "earthBaseToRoverComm.pb.h"
#include "roverToEarthBaseComm.pb.h"

namespace {
    constexpr auto MAX_PENDING_CONN { 1 };
    constexpr auto LISTEN_PORT { 5000 };
    constexpr auto ID_FRONT_LEFT_WHEEL { 0 };
    constexpr auto ID_FRONT_RIGHT_WHEEL { 1 };
    constexpr auto ID_REAR_LEFT_WHEEL { 2 };
    constexpr auto ID_REAR_RIGHT_WHEEL { 3 };

    constexpr char GROUP_REMOTE_INPUT_SOURCE[] = "COMMAND_INPUT_SOURCE";
    constexpr char TAG_SOURCE_TYPE_ID[] = "SOURCE_TYPE_ID";
}


OrionEngine::OrionEngine(QObject *parent)
    : QObject(parent),
      serialManager( new SerialManager(this) ),
      serialController( QSharedPointer<SerialController>::create() ),
      chassisModel ( new Orion::ChassisModel(this) ),
      sendFeedbackTimer( new QTimer(this) )
{
    setup_serial();
    setup_cmd_source();
    connections();
    sendFeedbackTimer->setInterval(150);
    sendFeedbackTimer->start();
}

OrionEngine::~OrionEngine()
{
    store_settings();
}

void OrionEngine::onRemoteMessageReceived(const QByteArray &message)
{
    if( !chassisModel )
        return;

    ORION_COMM::QUERY::Query cmd;
    cmd.ParseFromArray(message.data(), message.size());
    if( cmd.module() != ORION_COMM::DRIVE )
        return;

    switch( cmd.cmd() )
    {
    case ORION_COMM::CREATE:
        break;
    case ORION_COMM::READ:
        break;
    case ORION_COMM::UPDATE:
        parse_crud_update(cmd);
        break;
    case ORION_COMM::DELETE:
        break;
    default:
        break;
    }
}

void OrionEngine::onFeedbackTimerTimeout()
{
    auto feedbackData = chassisModel->getFeedbackData();
    commandSource->send(feedbackData);
}

void OrionEngine::setup_serial()
{
    QScopedPointer<DeviceFinder> finder(new ThreadedDeviceFinder( new JsonDeviceFactory, this));
    auto settings = QSharedPointer<SerialSettings>::create();
    finder->setSerialSettings(settings);
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

    chassisModel->setDriveAlgorithm(QSharedPointer<Orion::ProtobufDriveModeDirect>::create());
    chassisModel->setFeedbackGeneratorAlgorithm(QSharedPointer<Orion::ProtobufChassisFeedbackGenerator>::create());
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

    serialManager->setSettings(settings);
    serialManager->setDevices(devices);
    auto notConnected { serialManager->start() };
    echo_not_connected_devices(notConnected);
}

void OrionEngine::setup_cmd_source()
{
    const auto APP_NAME { QCoreApplication::applicationName() };
    const auto ORGANIZATION { QCoreApplication::organizationName() };
    QSettings in(ORGANIZATION, APP_NAME);
    in.beginGroup(GROUP_REMOTE_INPUT_SOURCE);
    const auto sourceType {(SOURCE_TYPE)in.value(TAG_SOURCE_TYPE_ID, (int)SOURCE_TYPE::TCP).toInt()};
    in.endGroup();

    QString sourceTypeText = "undefined";
    switch( sourceType ) {
    case SOURCE_TYPE::TCP:
        sourceTypeText = "TCP";
        commandSource.reset(new TcpInputSource(ORGANIZATION, APP_NAME ));
        break;
    case SOURCE_TYPE::GAMEPAD:
        sourceTypeText = "Gamepad";
        commandSource.reset(new GamepadInputSource(ORGANIZATION, APP_NAME));
        break;
    case SOURCE_TYPE::UNDEFINED:
    default:
        qDebug() << "Input source undefined. Exit";
        QCoreApplication::exit(1);
        return;
    }

    qDebug() << tr("Input source: %1(%2)").arg(sourceTypeText).arg((int)sourceType);
    store_settings();
}

void OrionEngine::connections()
{
    connect(commandSource.data(), &IfceInputSource::signalMessageReceived, this, &OrionEngine::onRemoteMessageReceived);
    connect(serialManager.data(), &SerialManager::serialError, this,
            [&](int id, const QString &errorString){
        qDebug() << QString("Error occured! DeviceId: %1 - error: %2").arg(id).arg(errorString);
    });

    connect(sendFeedbackTimer.data(), &QTimer::timeout, this, &OrionEngine::onFeedbackTimerTimeout);
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

void OrionEngine::store_settings()
{
    const auto APP_NAME { QCoreApplication::applicationName() };
    const auto ORGANIZATION { QCoreApplication::organizationName() };
    QSettings out(ORGANIZATION, APP_NAME , this);
    out.beginGroup(GROUP_REMOTE_INPUT_SOURCE);
    out.setValue(TAG_SOURCE_TYPE_ID, (int)commandSource->type());
    out.endGroup();
}

void OrionEngine::parse_crud_update(const ORION_COMM::QUERY::Query &query)
{
    switch( query.mode() )
    {
    case ORION_COMM::USER_CONTROLLED:
        parse_user_controlled_message(query);
        break;
    case ORION_COMM::SCRIPTED:
        break;
    case ORION_COMM::AUTONOMOUS:
        break;
    default:
        break;
    }
}

void OrionEngine::parse_user_controlled_message(const ORION_COMM::QUERY::Query &query)
{
    auto driveCmd = query.input();
    QByteArray serialzedDriveCmd(driveCmd.SerializeAsString().c_str(),
                                 driveCmd.ByteSize());
    chassisModel->updateState(serialzedDriveCmd);
    chassisModel->notifyAll();
}
