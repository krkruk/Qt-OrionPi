#include "JsonChassisFeedbackGenerator.h"
#include <QSharedPointer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QByteArray>
#include <QList>

#include "IfceWheelModel.h"
#include "OrionDriveSettings.h"


using namespace Orion;

class JsonChassisFeedbackGenerator::JsonGeneratorPrivate
{
    const QString KEY_ANG_VEL = OrionDriveSettings::instance()->getKeyAngularVelocity();
    const QString KEY_CURRENT = OrionDriveSettings::instance()->getKeyCurrent();
    const QString KEY_SINK_TEMP = OrionDriveSettings::instance()->getKeyHeatSinkTemperature();
    const QString KEY_PWM = OrionDriveSettings::instance()->getKeyPwm();
    const QString KEY_ERROR_CODE = OrionDriveSettings::instance()->getKeyErrorCode();


    friend class JsonChassisFeedbackGenerator;
public:
    ~JsonGeneratorPrivate() = default;
    void clear() { wheels.clear(); }
private:
    QList<QWeakPointer<IfceWheelModel>> wheels;
};


JsonChassisFeedbackGenerator::JsonChassisFeedbackGenerator()
    : priv{new JsonGeneratorPrivate}
{
}

JsonChassisFeedbackGenerator::~JsonChassisFeedbackGenerator()
{
}

void JsonChassisFeedbackGenerator::addInput(QWeakPointer<Orion::IfceWheelModel> model)
{
    priv->wheels.append(model);
}

QByteArray JsonChassisFeedbackGenerator::generate()
{
    const auto GROUP_KEY = OrionDriveSettings::instance()->getKeyGroup();
    QJsonArray wheelFeedback;
    for( QSharedPointer<Orion::IfceWheelModel> wheel : priv->wheels )
    {
        if( !wheel ) continue;

        QJsonObject obj;
        obj[priv->KEY_ANG_VEL] = wheel->getCurrentAngularVelocity();
        obj[priv->KEY_CURRENT] = wheel->getCurrent();
        obj[priv->KEY_SINK_TEMP] = wheel->getHeatSinkTemperature();
        obj[priv->KEY_PWM] = wheel->getPwm();
        obj[priv->KEY_ERROR_CODE] = wheel->getErrorCode();
        QJsonObject id;
        auto idTag { QString::number(wheel->getId()) };
        id[idTag] = obj;

        wheelFeedback.append(id);
    }

    QJsonObject feedbackData;
    feedbackData[GROUP_KEY] = wheelFeedback;

    return QJsonDocument(feedbackData).toJson(QJsonDocument::Compact);
}

void JsonChassisFeedbackGenerator::clear()
{
    priv->clear();
}
