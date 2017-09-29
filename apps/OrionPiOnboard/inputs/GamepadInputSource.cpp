#include "GamepadInputSource.h"
#include <QCoreApplication>
#include <QGamepadManager>
#include <QSettings>
#include <QGamepad>
#include <QTimer>
#include <QDebug>
#include <cmath>

#include "../protos/earthBaseToRoverComm.pb.h"
#include "../protos/roverToEarthBaseComm.pb.h"


namespace
{
    constexpr char GROUP_GAMEPAD_SETTINGS[] = "GAMEPAD_SETTINGS";
    constexpr char TAG_INPUT_PING_INTERVAL[] = "INPUT_PING_INTERVAL_MS";
    constexpr char TAG_REVERT_AXIS[] = "INPUT_REVERT_AXIS";
    constexpr char TAG_AXIS_MULTIPLIER[] = "INPUT_AXIS_MULTIPLIER";
    constexpr char TAG_AXIS_DEADZONE[] = "INPUT_AXIS_DEADZONE";

    const auto PING_INTERVAL { 50 };
    const auto REVERT_AXIS { true ? -1 : 1 };
    const auto INPUT_MULTIPLIER { 100 };
    const auto DEADZONE_PERCENT { .1 };
}

GamepadInputSource::GamepadInputSource(const QString &organization, const QString &application, QObject *parent)
    : IfceInputSource(parent),
      organization(organization),
      application(application),
      gamepadPinger(new QTimer(this))
{
    load_settings();
    setup_gamepad_pinger();
}

GamepadInputSource::~GamepadInputSource()
{
    store_settings();
}

void GamepadInputSource::send(const QByteArray &data)
{
    Q_UNUSED(data)
}

SOURCE_TYPE GamepadInputSource::type() const
{
    return SOURCE_TYPE::GAMEPAD;
}

void GamepadInputSource::onPingerTimeout()
{
    if( !gamepad ) {
        create_gamepad();
        return;
    }

    leftAxis = gamepad->axisLeftY();
    rightAxis = gamepad->axisRightY();

    leftAxis = process_input(leftAxis);
    rightAxis = process_input(rightAxis);

    broadcast_gamepad_results();
}

void GamepadInputSource::load_settings()
{
    QSettings in(organization, application);
    in.beginGroup(GROUP_GAMEPAD_SETTINGS);
    pingInterval = in.value(TAG_INPUT_PING_INTERVAL, PING_INTERVAL).toInt();
    revertAxis = in.value(TAG_REVERT_AXIS, REVERT_AXIS).toInt();
    inputMultiplier = in.value(TAG_AXIS_MULTIPLIER, INPUT_MULTIPLIER).toInt();
    deadZone = in.value(TAG_AXIS_DEADZONE, DEADZONE_PERCENT).toDouble();
    in.endGroup();

    store_settings();
}

void GamepadInputSource::setup_gamepad_pinger()
{
    if( pingInterval <= 0 )
        QCoreApplication::exit(2);

    gamepadPinger->setInterval(pingInterval);
    connect(gamepadPinger.data(), &QTimer::timeout, this, &GamepadInputSource::onPingerTimeout);
    gamepadPinger->start();
}

void GamepadInputSource::create_gamepad()
{
    auto gamepads {QGamepadManager::instance()->connectedGamepads()};
    if( gamepads.isEmpty() )
        return;

    gamepad = new QGamepad(*gamepads.begin(), this);
}

double GamepadInputSource::process_input(double input) const
{
    return revertAxis * inputMultiplier * apply_dead_zone(input);
}

double GamepadInputSource::apply_dead_zone(double input) const
{
    return std::abs(input) > deadZone ? input : 0.0f;
}

void GamepadInputSource::broadcast_gamepad_results()
{
    ORION_COMM::Command cmd;
    cmd.set_cmdtype(ORION_COMM::Drive);
    ORION_COMM::Chassis *chassis = cmd.mutable_drive();
    chassis->set_leftrowangularvelocity(leftAxis);
    chassis->set_rightrowangularvelocity(rightAxis);

    auto serialized = cmd.SerializeAsString();
    QByteArray data(serialized.data(), serialized.size());
    emit signalMessageReceived(data);
}

void GamepadInputSource::store_settings()
{
    QSettings out(organization, application);
    out.beginGroup(GROUP_GAMEPAD_SETTINGS);
    out.setValue(TAG_INPUT_PING_INTERVAL, pingInterval);
    out.setValue(TAG_REVERT_AXIS, revertAxis);
    out.setValue(TAG_AXIS_MULTIPLIER, inputMultiplier);
    out.setValue(TAG_AXIS_DEADZONE, deadZone);
    out.endGroup();
}
