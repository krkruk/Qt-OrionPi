#include "ProtobufChassisFeedbackGenerator.h"
#include <QByteArray>
#include <QList>
#include <QDebug>
#include "interface/IfceWheelModel.h"
#include "earthBaseToRoverComm.pb.h"
#include "roverToEarthBaseComm.pb.h"


using namespace Orion;
class ProtobufChassisFeedbackGenerator::FeedbackGeneratorPrivate
{
    friend class ProtobufChassisFeedbackGenerator;
public:
    ~FeedbackGeneratorPrivate() = default;
    void clear() { wheels.clear(); }
private:
    QList<QWeakPointer<IfceWheelModel>> wheels;
};

ProtobufChassisFeedbackGenerator::ProtobufChassisFeedbackGenerator()
    : priv{new FeedbackGeneratorPrivate}
{    
}

ProtobufChassisFeedbackGenerator::~ProtobufChassisFeedbackGenerator()
{
}

void Orion::ProtobufChassisFeedbackGenerator::addInput(QWeakPointer<IfceWheelModel> model)
{
    priv->wheels.append(model);
}

QByteArray Orion::ProtobufChassisFeedbackGenerator::generate()
{
    ORION_COMM::REPLY::Reply msg;
    ORION_COMM::REPLY::DriveModuleTelemetry *chassis { msg.mutable_chassis() };
    msg.set_reply_type(ORION_COMM::UPDATE);
    msg.set_module(ORION_COMM::DRIVE);

    for( QSharedPointer<Orion::IfceWheelModel> wheel : priv->wheels )
    {
        auto *wheelTelemetry = chassis->add_wheel();
        wheelTelemetry->set_id(wheel->getId());
        wheelTelemetry->set_angularvelocity(wheel->getCurrentAngularVelocity());
        wheelTelemetry->set_current(wheel->getCurrent());
        wheelTelemetry->set_heatsinktemperature(wheel->getHeatSinkTemperature());
        wheelTelemetry->set_pwm(wheel->getPwm());
        wheelTelemetry->set_errorcode(wheel->getErrorCode());
    }
    auto serialized { msg.SerializeAsString() };
    return QByteArray(serialized.c_str(), msg.ByteSize());
}

void Orion::ProtobufChassisFeedbackGenerator::clear()
{
    priv->clear();
}
