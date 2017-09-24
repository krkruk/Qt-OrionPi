#include "JsonDriveModeDirect.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include "DriveConstants.h"
#include "settings/DriveSettings.h"
#include <QDebug>


using namespace Orion;

class JsonDriveModeDirect::DriveModeDirectPrivate
{
    friend class JsonDriveModeDirect;
    const QString KEY_LEFT_WHEEL_ROW = DriveSettings::instance()->getDriveMode(
                CONST::SETTINGS::DRIVE::MODE::DIRECT::KEY::LEFT_WHEEL_ROW);
    const QString KEY_RIGHT_WHEEL_ROW = DriveSettings::instance()->getDriveMode(
                CONST::SETTINGS::DRIVE::MODE::DIRECT::KEY::RIGHT_WHEEL_ROW);
public:
    DriveModeDirectPrivate() {}
    ~DriveModeDirectPrivate() {}


private:
    double leftRow {0.0};
    double rightRow {0.0};
};

JsonDriveModeDirect::JsonDriveModeDirect()
    : member(new DriveModeDirectPrivate)
{
}

JsonDriveModeDirect::~JsonDriveModeDirect()
{
}

void JsonDriveModeDirect::processInput(const QByteArray &rawData)
{
    QJsonParseError parseErr;
    auto doc { QJsonDocument::fromJson(rawData, &parseErr) };
    if( parseErr.error != QJsonParseError::NoError )
        qDebug() << "processInput(): parsing error: "<<parseErr.errorString() << ":"<<rawData;

    QJsonObject obj { doc.object() };
    member->leftRow = obj.value(member->KEY_LEFT_WHEEL_ROW).toDouble();
    member->rightRow = obj.value(member->KEY_RIGHT_WHEEL_ROW).toDouble();
}

double JsonDriveModeDirect::getValue(int wheel)
{
    /*
     * Left row of wheels is number in 0,2,4... manner (even numbers)
     * Right row of wheels is number in 1,3,5... manner (odd numbers)
     */
    return isLeftRow(wheel) ? member->leftRow : member->rightRow;
}
