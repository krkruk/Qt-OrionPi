#include "WheelModel.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>

#include <QDebug>

#include "exceptions/ParsingException.h"
#include "include/interface/IfceSerialObserver.h"
#include "settings/DriveSettings.h"

using namespace Orion;

class WheelModel::WheelModelPrivate
{
    const QString KEY_ANG_VEL = DriveSettings::instance()->getKeyAngularVelocity();
    const QString KEY_CURRENT = DriveSettings::instance()->getKeyCurrent();
    const QString KEY_SINK_TEMP = DriveSettings::instance()->getKeyHeatSinkTemperature();
    const QString KEY_PWM = DriveSettings::instance()->getKeyPwm();
    const QString KEY_ERROR_CODE = DriveSettings::instance()->getKeyErrorCode();

    friend class WheelModel;
    WheelModel *model;
public:
    WheelModelPrivate(WheelModel *model) : model {model}{}
    ~WheelModelPrivate(){}
    void parse(const QByteArray &data);

    void updateAngularVelocity(double currentAngularVelocity);

    double getCurrentAngularVelocity() const;
    double getCurrent() const;
    double getHeatSinkTemperature() const;

    void addObserver(QWeakPointer<IfceSerialObserver> observer);
    bool delObserver(QWeakPointer<IfceSerialObserver> observer);
    void notifyObservers();


private:
    QByteArray exportDataToSerial();

    double currentAngularVelocity { 0.0 };
    double expectedAngularVelocity { 0.0 };
    double current { 0.0 };
    double heatSinkTemp { 0.0 };
    int pwm { 0 };
    int errorCode { 0 };

    QList<QWeakPointer<IfceSerialObserver>> observers;
};


void WheelModel::WheelModelPrivate::parse(const QByteArray &data)
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

void WheelModel::WheelModelPrivate::addObserver(QWeakPointer<IfceSerialObserver> observer)
{
    observers.append(observer);
}

bool WheelModel::WheelModelPrivate::delObserver(QWeakPointer<IfceSerialObserver> observer)
{
    return observers.removeOne(observer);
}

void WheelModel::WheelModelPrivate::notifyObservers()
{
    const auto data { exportDataToSerial() };
    for( auto &observer : observers )
        if( auto effector = observer.toStrongRef() )
            effector->send(model->getId(), data);
}

QByteArray WheelModel::WheelModelPrivate::exportDataToSerial()
{
    QJsonObject obj;
    obj[KEY_ANG_VEL] = (int)model->getExpectedAngularVelocity();
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

void WheelModel::updateModel(const QByteArray &line)
{
    try {
        member->parse(line);
    } catch( ParsingException &e ) {
        qDebug() << "WheelModel::update():" << e.what();
    }
}

void WheelModel::addObserver(QWeakPointer<IfceSerialObserver> observer)
{
    member->addObserver(observer);
}

bool WheelModel::delObserver(QWeakPointer<IfceSerialObserver> observer)
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

double WheelModel::getExpectedAngularVelocity() const
{
    return member->expectedAngularVelocity;
}

double WheelModel::getCurrentAngularVelocity() const
{
    return member->getCurrentAngularVelocity();
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

