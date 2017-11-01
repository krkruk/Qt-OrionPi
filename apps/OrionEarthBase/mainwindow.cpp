#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QGamepadManager>
#include <QGamepad>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QTimer>
#include <QDebug>
#include <QMap>

#include "NetworkSettings.h"
#include "TcpSocket.h"

#include <google/protobuf/util/json_util.h>
#include "protocolEnums.pb.h"
#include "earthBaseToRoverComm.pb.h"
#include "roverToEarthBaseComm.pb.h"

namespace {
    const QHostAddress bindAddress = QHostAddress::LocalHost;
    constexpr int bindPort { 5001 };

    constexpr int GAMEPAD_REFRESH_INTERVAL { 25 };
    constexpr auto MIN_MULTIPLIER { -1 };
    constexpr auto REVERT_AXIS { true ? -1 : 1 };
    constexpr auto DEAD_ZONE_PERCENT { 0.10 };

    const QString LEFT_WHEEL_ROW { "LROW" };
    const QString RIGHT_WHEEL_ROW { "RROW" };

    constexpr char ORGAZNIATION[] { "Orion Project" };
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    socket(new TcpSocket(bindAddress, bindPort, this)),
    gamepadTimer(new QTimer(this))
{
    ui->setupUi(this);
    ui->gridLayout->setColumnStretch(0, 1);
    ui->gridLayout->setColumnStretch(1, 1);
    inputMultiplier = ui->spinBoxMaximaLimit->value();
    display_available_gamepads();
    connections();
    on_pushButtonAcceptGamepad_clicked();
    setup_gamepad_timer();
    start_connection();
}

MainWindow::~MainWindow()
{
    leftAxis = 0;
    rightAxis = 0;
    write_gamepad_input_to_server();
    delete ui;
}

void MainWindow::onRemoteMessageReceived(const QByteArray &message)
{
    parse_feedback_data(message);
}

void MainWindow::onGamepadIntervalTimeout()
{
    if( gamepad.isNull() )
        return;

    leftAxis = gamepad->axisLeftY();
    rightAxis = gamepad->axisRightY();

    leftAxis = process_input(leftAxis);
    rightAxis = process_input(rightAxis);

    apply_gamepad_to_widget();
    write_gamepad_input_to_server();
}

void MainWindow::onMaximumSliderChanged(int value)
{
    inputMultiplier = value;
    ui->verticalSliderLeftAxis->setMinimum(MIN_MULTIPLIER * inputMultiplier);
    ui->verticalSliderLeftAxis->setMaximum(inputMultiplier);
    ui->verticalSliderRightAxis->setMinimum(MIN_MULTIPLIER * inputMultiplier);
    ui->verticalSliderRightAxis->setMaximum(inputMultiplier);
}

void MainWindow::connections()
{
    connect(socket.data(), &IfceIpSocket::signalMessageReceived, this, &MainWindow::onRemoteMessageReceived);
    connect(gamepadTimer.data(), &QTimer::timeout, this, &MainWindow::onGamepadIntervalTimeout);
    connect(ui->spinBoxMaximaLimit, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MainWindow::onMaximumSliderChanged);
}

void MainWindow::setup_gamepad_timer()
{
    gamepadTimer->setInterval(GAMEPAD_REFRESH_INTERVAL);
    gamepadTimer->start();
}

void MainWindow::start_connection()
{
    auto appName { QApplication::applicationName() };
    QScopedPointer<NetworkSettings> net(new NetworkSettings(ORGAZNIATION, appName, this));
    auto ip { net->getConnectToAddress() };
    auto port { net->getConnectToPort() };
    socket->connectToHost(ip, port);
}

void MainWindow::display_available_gamepads()
{
    ui->comboBoxGamepadSelection->clear();
    const auto avaialbleGamepads = QGamepadManager::instance()->connectedGamepads();

    for( auto gamepadId : avaialbleGamepads )
        ui->comboBoxGamepadSelection->addItem(QString::number(gamepadId));
}

void MainWindow::parse_feedback_data(const QByteArray &data)
{
//    QJsonDocument doc { QJsonDocument::fromJson(QString(data).toLocal8Bit()) };
//    QJsonObject obj { doc.object() };
//    QJsonArray driveRelated = obj["DRIVE"].toArray();

//    for( auto wheelDict : driveRelated ) {
//        auto wheel = wheelDict.toObject();
//        if( wheel.keys().size() == 0 ) continue;
//        const auto wheelName = wheel.keys()[0];
//        auto wheelValue = wheel[wheelName].toObject();
//        const auto angVelocity = wheelValue["ROT"].toDouble();
//        const auto heatSinkTemp = wheelValue["TEMP"].toDouble();
//        const auto current = wheelValue["CUR"].toDouble();
//        const auto pwm = wheelValue["PWM"].toInt();
//        const auto errorCode = wheelValue["ERR"].toInt();

//        apply_data_to_feedback_widget(wheelName,
//                                      angVelocity,
//                                      heatSinkTemp,
//                                      current,
//                                      pwm,
//                                      errorCode);
//    }

    ORION_COMM::REPLY::Reply msg;
    msg.ParseFromArray(data.data(), data.size());
    std::string json;
    google::protobuf::util::MessageToJsonString(msg, &json);

    if( msg.reply_type() != ORION_COMM::UPDATE )
        return;
    if( msg.module() != ORION_COMM::DRIVE )
        return;

    auto wheels = msg.chassis().wheel();
    using WheelById = QMap<int, const ORION_COMM::REPLY::WheelTelemetry*>;
    WheelById sorted;
    for( const ORION_COMM::REPLY::WheelTelemetry &wheel : wheels ) {
        sorted.insert(wheel.id(), &wheel);
    }

    for( auto it = sorted.cbegin(); it != sorted.cend(); ++it ) {
        const auto wheelName = QString::number(it.key());
        const auto angVelocity = it.value()->angularvelocity();
        const auto heatSinkTemp = it.value()->heatsinktemperature();
        const auto current = it.value()->current();
        const auto pwm = it.value()->pwm();
        const auto errorCode = it.value()->errorcode();
        apply_data_to_feedback_widget(wheelName,
                                      angVelocity,
                                      heatSinkTemp,
                                      current,
                                      pwm,
                                      errorCode);
    }
}

void MainWindow::apply_data_to_feedback_widget(const QString &name, double angularVelocity, double heatSinkTemp, double current, int pwm, int errorCode)
{
    if( !feedbackWidgets.contains(name) ) {
        auto widget = QPointer<DriveFeedbackWidget>(new DriveFeedbackWidget(this));
        ui->gridLayout->addWidget(widget.data());
        feedbackWidgets.insert(name, widget);
    }

    auto widget = feedbackWidgets.value(name);
    widget->setWheelName(QString("Wheel %1").arg(name));
    widget->setAngularVelocity(angularVelocity);
    widget->setHeatSinkTemperature(heatSinkTemp);
    widget->setCurrent(current);
    widget->setPwm(pwm);
    widget->setErrorCode(errorCode);
}

double MainWindow::process_input(double input) const
{
    return REVERT_AXIS * inputMultiplier * apply_dead_zone(input);
}

double MainWindow::apply_dead_zone(double input) const
{
    return std::abs(input) > DEAD_ZONE_PERCENT ? input : 0.0;
}

void MainWindow::apply_gamepad_to_widget()
{
    ui->verticalSliderLeftAxis->setValue(leftAxis);
    ui->verticalSliderRightAxis->setValue(rightAxis);
    ui->labelLeftAxis->setText(QString::number(leftAxis));
    ui->labelRightAxis->setText(QString::number(rightAxis));
}

void MainWindow::write_gamepad_input_to_server()
{
//    QJsonObject obj{
//        QPair<QString, QJsonValue>(LEFT_WHEEL_ROW, leftAxis),
//        QPair<QString, QJsonValue>(RIGHT_WHEEL_ROW, rightAxis),
//    };
//    QJsonDocument doc(obj);

    ORION_COMM::QUERY::Query cmd;
    cmd.set_cmd(ORION_COMM::UPDATE);
    cmd.set_mode(ORION_COMM::USER_CONTROLLED);
    cmd.set_module(ORION_COMM::DRIVE);

    ORION_COMM::QUERY::InputDevice *drive = cmd.mutable_input();
    drive->set_x_axis_0(leftAxis);
    drive->set_y_axis_0(rightAxis);
    if( socket )
        socket->send(cmd.SerializeAsString());
}

void MainWindow::on_pushButtonAcceptGamepad_clicked()
{
    auto currentGamepadName = ui->comboBoxGamepadSelection->currentText();
    if( currentGamepadName.isEmpty() ) return;
    gamepad.reset(new QGamepad(currentGamepadName.toInt(), this));
}

void MainWindow::on_action_Refresh_triggered()
{
    gamepad.reset();
    display_available_gamepads();
}

void MainWindow::on_action_Settings_triggered()
{
    auto appName { QApplication::applicationName() };
    QScopedPointer<NetworkSettings> net(new NetworkSettings(ORGAZNIATION, appName, this));
    const auto result { net->exec() };

    if( result != QDialog::Accepted )
        return;

    auto ip { net->getConnectToAddress() };
    auto port { net->getConnectToPort() };
    socket->connectToHost(ip, port);
}
