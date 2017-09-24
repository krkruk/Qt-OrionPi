#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QJsonArray>
#include <QScopedPointer>
#include <QSharedPointer>

#include "MockWheelObserver.h"
#include "include/interface/IfceSerialModel.h"
#include "interface/IfceChassisFeedbackGenerator.h"
#include "WheelModel.h"
#include "ChassisModel.h"
#include "interface/IfceDriveMode.h"
#include "JsonDriveModeDirect.h"
#include "JsonChassisFeedbackGenerator.h"
#include "settings/DriveSettings.h"
#include "DriveConstants.h"

static const QString KEY_ANG_VEL = "w";


class TestDriveModel : public QObject
{
    Q_OBJECT
    const QString KEY_ANG_VEL = DriveSettings::instance()->getKeyAngularVelocity();
    const QString KEY_CURRENT = DriveSettings::instance()->getKeyCurrent();
    const QString KEY_SINK_TEMP = DriveSettings::instance()->getKeyHeatSinkTemperature();
    const QString KEY_PWM = DriveSettings::instance()->getKeyPwm();
    const QString KEY_ERROR_CODE = DriveSettings::instance()->getKeyErrorCode();

    const QString KEY_LEFT_WHEEL_ROW = DriveSettings::instance()->getDriveMode(
                CONST::SETTINGS::DRIVE::MODE::DIRECT::KEY::LEFT_WHEEL_ROW);
    const QString KEY_RIGHT_WHEEL_ROW = DriveSettings::instance()->getDriveMode(
                CONST::SETTINGS::DRIVE::MODE::DIRECT::KEY::RIGHT_WHEEL_ROW);
public:
    TestDriveModel();

    double getAngularVelocity(const QByteArray &data)
    {
        QJsonDocument doc { QJsonDocument::fromJson(data) };
        QJsonObject obj { doc.object() };
        return obj.value(KEY_ANG_VEL).toDouble();
    }

private Q_SLOTS:
    void test_get_wheel_id();
    void test_parse_raw_data_get_ang_velocity_current_temperature();
    void test_wheel_model_notification_ability();
    void test_chassis_model_data_acquisition();
    void test_drive_mode_direct_parsing();
    void test_set_drive_mode_algorithm_direct();
    void test_feedback_generator();
    void test_full_json_feedback_gen();
};

TestDriveModel::TestDriveModel()
{
}

void TestDriveModel::test_get_wheel_id()
{
    constexpr auto wheelId { 0 };
    QScopedPointer<IfceSerialModel> wheel { new Orion::WheelModel(wheelId) };
    QCOMPARE( wheel->getId(), wheelId );
}

void TestDriveModel::test_parse_raw_data_get_ang_velocity_current_temperature()
{
    constexpr auto wheelId { 0 };
    constexpr auto omega { 100.0 };
    constexpr auto current { 5.0 };
    constexpr auto temperature { 88.5 };
    constexpr auto errorCode { 1 };
    constexpr auto pwm { 244 };
    const QByteArray data(QString("{\"ID\":0, \"%1\": %2, \"%3\": %4, \"%5\": %6, \"%7\": %8, \"%9\": %10}")
                          .arg(KEY_ANG_VEL).arg(omega)
                          .arg(KEY_CURRENT).arg(current)
                          .arg(KEY_SINK_TEMP).arg(temperature)
                          .arg(KEY_PWM).arg(pwm)
                          .arg(KEY_ERROR_CODE).arg(errorCode).toLatin1());
    QSharedPointer<IfceSerialModel> wheel { new Orion::WheelModel(wheelId) };
    auto *solidWheel = dynamic_cast<Orion::WheelModel*>(wheel.data());
    wheel->updateModel(data);

    QCOMPARE( solidWheel->getCurrentAngularVelocity(), omega );
    QCOMPARE( solidWheel->getCurrent(), current );
    QCOMPARE( solidWheel->getHeatSinkTemperature(), temperature );
    QCOMPARE( solidWheel->getPwm(), pwm );
    QCOMPARE( solidWheel->getErrorCode(), errorCode );
}

void TestDriveModel::test_wheel_model_notification_ability()
{
    constexpr auto frontLeftId { 0 };
    constexpr auto omega { 100.0 };
    constexpr auto current { 5.0 };
    constexpr auto temperature { 88.5 };
    const QByteArray data(QString("{\"ID\":0,\"%1\": %2,\"%3\":%4,\"%5\":%6}")
                          .arg(KEY_ANG_VEL).arg(omega)
                          .arg(KEY_CURRENT).arg(current)
                          .arg(KEY_SINK_TEMP).arg(temperature).toLatin1());

    auto frontLeftWheel { QSharedPointer<Orion::WheelModel>::create(frontLeftId) };
    QSharedPointer<MockWheelObserver> observer { QSharedPointer<MockWheelObserver>::create() };
    frontLeftWheel->addObserver(observer);
    frontLeftWheel->updateModel(data);
    QCOMPARE( frontLeftWheel->getCurrentAngularVelocity(), omega );
}

void TestDriveModel::test_chassis_model_data_acquisition()
{
    constexpr auto frontLeftId { 0 };
    constexpr auto frontRightId { 1 };
    constexpr auto rearLeftId { 2 };
    constexpr auto rearRightId { 3 };
    constexpr auto value { 100.0 };

    auto frontLeftWheel { QSharedPointer<Orion::WheelModel>::create(frontLeftId) };
    auto frontRightWheel { QSharedPointer<Orion::WheelModel>::create(frontRightId) };
    auto rearLeftWheel { QSharedPointer<Orion::WheelModel>::create(rearLeftId) };
    auto rearRightWheel { QSharedPointer<Orion::WheelModel>::create(rearRightId) };

    Orion::ChassisModel orionChassis;
    orionChassis.addWheel(frontLeftWheel);
    orionChassis.addWheel(frontRightWheel);
    orionChassis.addWheel(rearLeftWheel);
    orionChassis.addWheel(rearRightWheel);

    const QString data(QString("{\"ID\":%4, \"%1\": 100.0, \"%2\":5.0, \"%3\":88.5}")
                       .arg(KEY_ANG_VEL).arg(KEY_CURRENT).arg(KEY_SINK_TEMP));
    QSharedPointer<MockWheelObserver> observer { QSharedPointer<MockWheelObserver>::create() };
    frontLeftWheel->addObserver(observer);
    frontLeftWheel->updateModel(data.arg(frontLeftId).toLatin1());

    frontRightWheel->addObserver(observer);
    frontRightWheel->updateModel(data.arg(frontRightId).toLatin1());

    rearLeftWheel->addObserver(observer);
    rearLeftWheel->updateModel(data.arg(rearLeftId).toLatin1());

    rearRightWheel->addObserver(observer);
    rearRightWheel->updateModel(data.arg(rearRightId).toLatin1());

    orionChassis.notifyAll();
    QCOMPARE( frontLeftWheel->getCurrentAngularVelocity(), value );
    QCOMPARE( frontRightWheel->getCurrentAngularVelocity(), value );
    QCOMPARE( rearLeftWheel->getCurrentAngularVelocity(), value );
    QCOMPARE( rearRightWheel->getCurrentAngularVelocity(), value );
}

void TestDriveModel::test_drive_mode_direct_parsing()
{
    QScopedPointer<Orion::IfceDriveMode> drive { new Orion::JsonDriveModeDirect };
    const QByteArray turnLeft( R"({"RROW":100.0, "LROW":-100.0})");
    drive->processInput(turnLeft);
    auto frontLeft { drive->getValue(Orion::JsonDriveModeDirect::LEFT_0) };
    auto frontRight { drive->getValue(Orion::JsonDriveModeDirect::RIGHT_0) };
    QCOMPARE( frontLeft, -100.0 );
    QCOMPARE( frontRight, 100.0 );
}

void TestDriveModel::test_set_drive_mode_algorithm_direct()
{
    constexpr auto frontLeftId { 0 };
    constexpr auto frontRightId { 1 };
    auto frontLeftWheel { QSharedPointer<Orion::WheelModel>::create(frontLeftId) };
    auto frontRightWheel { QSharedPointer<Orion::WheelModel>::create(frontRightId) };

    Orion::ChassisModel orionChassis;
    orionChassis.setDriveAlgorithm( QSharedPointer<Orion::JsonDriveModeDirect>::create() );
    orionChassis.addWheel(frontLeftWheel);
    orionChassis.addWheel(frontRightWheel);
    QSharedPointer<MockWheelObserver> observer { QSharedPointer<MockWheelObserver>::create() };
    frontLeftWheel->addObserver(observer);
    frontRightWheel->addObserver(observer);

    const QByteArray turnLeft( R"({"RROW":100.0, "LROW":-100.0})");
    orionChassis.updateState(turnLeft);
    orionChassis.notifyAll();
    QCOMPARE( frontLeftWheel->getExpectedAngularVelocity(), -100.0 );
    QCOMPARE( frontRightWheel->getExpectedAngularVelocity(), 100.0 );
}

void TestDriveModel::test_feedback_generator()
{
    constexpr auto frontLeftId { 0 };
    const auto omega{100};
    const auto current{23.3};
    const auto heatSinkTemp{45.5};
    const auto pwm{200};
    const auto errorCode{0};

    QJsonObject data;
    data[KEY_ANG_VEL] = omega;
    data[KEY_CURRENT] = current;
    data[KEY_SINK_TEMP] = heatSinkTemp;
    data[KEY_PWM] = pwm;
    data[KEY_ERROR_CODE] = errorCode;
    QJsonObject dataWithId;
    dataWithId[QString::number(frontLeftId)] = data;
    QJsonObject toBeGenerated;
    toBeGenerated[DriveSettings::instance()->getKeyGroup()] = QJsonArray{dataWithId};

    auto frontLeftWheel { QSharedPointer<Orion::WheelModel>::create(frontLeftId) };
    frontLeftWheel->updateModel(QJsonDocument(data).toJson(QJsonDocument::Compact));

    QSharedPointer<Orion::IfceChassisFeedbackGenerator> generator { QSharedPointer<Orion::JsonChassisFeedbackGenerator>::create() };
    generator->addInput(frontLeftWheel);
    auto generated = generator->generate();

    QJsonDocument doc(QJsonDocument::fromJson(generated));
    QJsonObject obj{doc.object()};
    QCOMPARE( obj, toBeGenerated );
}

void TestDriveModel::test_full_json_feedback_gen()
{
    constexpr auto frontLeftId { 0 };
    const auto omega{100};
    const auto current{23.3};
    const auto heatSinkTemp{45.5};
    const auto pwm{200};
    const auto errorCode{0};

    QJsonObject data;
    data[KEY_ANG_VEL] = omega;
    data[KEY_CURRENT] = current;
    data[KEY_SINK_TEMP] = heatSinkTemp;
    data[KEY_PWM] = pwm;
    data[KEY_ERROR_CODE] = errorCode;
    QJsonObject dataWithId;
    dataWithId[QString::number(frontLeftId)] = data;
    QJsonObject toBeGenerated;
    toBeGenerated[DriveSettings::instance()->getKeyGroup()] = QJsonArray{dataWithId};

    auto frontLeftWheel { QSharedPointer<Orion::WheelModel>::create(frontLeftId) };

    Orion::ChassisModel chassis;
    chassis.setDriveAlgorithm(QSharedPointer<Orion::JsonDriveModeDirect>::create());
    chassis.setFeedbackGeneratorAlgorithm(QSharedPointer<Orion::JsonChassisFeedbackGenerator>::create());
    chassis.addWheel(frontLeftWheel);
    QSharedPointer<MockWheelObserver> observer { QSharedPointer<MockWheelObserver>::create() };
    frontLeftWheel->addObserver(observer);
    frontLeftWheel->updateModel(QJsonDocument(data).toJson(QJsonDocument::Compact));
    auto dataToCompare = chassis.getFeedbackData();
    QJsonDocument genDocument {QJsonDocument::fromJson(dataToCompare)};
    QJsonObject objToCompare {genDocument.object()};
    QCOMPARE( objToCompare, toBeGenerated );
}


QTEST_MAIN(TestDriveModel)

#include "tst_testdrivemodel.moc"
