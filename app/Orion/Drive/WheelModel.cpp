#include "WheelModel.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>

#include <QDebug>

#include "exceptions/ParsingException.h"
#include "include/IfceSerialObserver.h"
#include "../../OrionDriveSettings.h"

using namespace Orion;

class WheelModel::WheelModelPrivate
{
    const QString KEY_ANG_VEL = OrionDriveSettings::instance()->getKeyAngularVelocity();
    const QString KEY_CURRENT = OrionDriveSettings::instance()->getKeyCurrent();
    const QString KEY_SINK_TEMP = OrionDriveSettings::instance()->getKeyHeatSinkTemperature();
    const QString KEY_PWM = OrionDriveSettings::instance()->getKeyPwm();
    const QString KEY_ERROR_CODE = OrionDriveSettings::instance()->getKeyErrorCode();

    friend class WheelModel;
    WheelModel *model;
public:
    WheelModelPrivate(WheelModel *model) : model {model}{}
    ~WheelModelPrivate(){}
    void parse(const QByteArray &data) throw(ParsingException);

    void updateAngularVelocity(double currentAngularVelocity);

    double getCurrentAngularVelocity() const;
    double getCurrent() const;
    double getHeatSinkTemperature() const;

    void addObserver(IfceSerialObserver *observer);
    bool delObserver(IfceSerialObserver *observer);
    void notifyObservers();


private:

    QByteArray exportData();
    double currentAngularVelocity { 0.0 };
    double expectedAngularVelocity { 0.0 };
    double current { 0.0 };
    double heatSinkTemp { 0.0 };
    int pwm { 0 };
    int errorCode { 0 };

    QList<IfceSerialObserver*> observers;
};


void WheelModel::WheelModelPrivate::parse(const QByteArray &data) throw(ParsingException)
{
    QJsonParseError parseError;
    auto doc { QJsonDocument::fromJson(data, &parseError) };
    if( parseError.error != QJsonParseError::NoError )
        throw ParsingException(QString("%1: %2")
                               .arg("WheelModel::WheelModelPrivate::parse()")
                               .arg(parseError.errorString()));
    QJsonObject obj { doc.object() };

    currentAngularVelocity = obj.value(KEY_ANG_VEL).toDouble();
    current = obj.value(KEY_CURRENT).toDouble();
    heatSinkTemp = obj.value(KEY_SINK_TEMP).toDouble();
    pwm = obj.value(KEY_PWM).toInt();
    errorCode = obj.value(KEY_ERROR_CODE).toInt();
}

void WheelModel::WheelModelPrivate::updateAngularVelocity(double angularVelocity)
{
    expectedAngularVelocity = angularVelocity;
}

double WheelModel::WheelModelPrivate::getCurrentAngularVelocity() const
{
    return currentAngularVelocity;
}

double WheelModel::WheelModelPrivate::getCurrent() const
{
    return current;
}

double WheelModel::WheelModelPrivate::getHeatSinkTemperature() const
{
    return heatSinkTemp;
}

void WheelModel::WheelModelPrivate::addObserver(IfceSerialObserver *observer)
{
    observers.append(observer);
}

bool WheelModel::WheelModelPrivate::delObserver(IfceSerialObserver *observer)
{
    return observers.removeOne(observer);
}

void WheelModel::WheelModelPrivate::notifyObservers()
{
    const auto data { exportData() };
    for( auto &observer : observers )
        observer->send(model->getId(), data);
}

QByteArray WheelModel::WheelModelPrivate::exportData()
{
    QJsonObject obj;
    obj[KEY_ANG_VEL] = getCurrentAngularVelocity();
    obj[KEY_CURRENT] = getCurrent();
    obj[KEY_SINK_TEMP] = getHeatSinkTemperature();
    obj[KEY_PWM] = this->pwm;
    obj[KEY_ERROR_CODE] = this->errorCode;
    QJsonDocument doc(obj);
    return doc.toJson(QJsonDocument::Compact);
}



WheelModel::WheelModel(int id)
    : IfceWheelModel(id),
      member{ new WheelModelPrivate(this) }
{
}

WheelModel::~WheelModel()
{
}

void WheelModel::update(const QByteArray &line)
{
    try {
        member->parse(line);
    } catch( ParsingException &e ) {
        qDebug() << "WheelModel::update():" << e.what();
    }
}

void WheelModel::addObserver(IfceSerialObserver *observer)
{
    member->addObserver(observer);
}

bool WheelModel::delObserver(IfceSerialObserver *observer)
{
    return member->delObserver(observer);
}

void WheelModel::notifyObservers()
{
    member->notifyObservers();
}

void WheelModel::updateAngularVelocity(double angularVelocity)
{
    member->updateAngularVelocity(angularVelocity);
}

double WheelModel::getCurrentAngularVelocity() const
{
    return member->getCurrentAngularVelocity();
}

double WheelModel::getExpectedAngularVelocity() const
{
    return member->expectedAngularVelocity;
}

double WheelModel::getCurrent() const
{
    return member->getCurrent();
}

double WheelModel::getHeatSinkTemperature() const
{
    return member->getHeatSinkTemperature();
}

int WheelModel::getPwm() const
{
    return member->pwm;
}

int WheelModel::getErrorCode() const
{
    return member->errorCode;
}

