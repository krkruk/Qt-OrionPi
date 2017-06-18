#include "DriveModeDirect.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include "DriveConstants.h"
#include "../../OrionDriveSettings.h"
#include <QDebug>


using namespace Orion;

class DriveModeDirect::DriveModeDirectPrivate
{
    friend class DriveModeDirect;
    const QString KEY_LEFT_WHEEL_ROW = OrionDriveSettings::instance()->getDriveMode(
                CONST::SETTINGS::DRIVE::MODE::DIRECT::KEY::LEFT_WHEEL_ROW);
    const QString KEY_RIGHT_WHEEL_ROW = OrionDriveSettings::instance()->getDriveMode(
                CONST::SETTINGS::DRIVE::MODE::DIRECT::KEY::RIGHT_WHEEL_ROW);
public:
    DriveModeDirectPrivate() {}
    ~DriveModeDirectPrivate() {}


private:
    double leftRow {0.0};
    double rightRow {0.0};
};

DriveModeDirect::DriveModeDirect()
    : member(new DriveModeDirectPrivate)
{
}

DriveModeDirect::~DriveModeDirect()
{
}

void DriveModeDirect::processInput(const QByteArray &rawData)
{
    QJsonParseError parseErr;
    auto doc { QJsonDocument::fromJson(rawData, &parseErr) };
    if( parseErr.error != QJsonParseError::NoError )
        qDebug() << "processInput(): parsing error: "<<parseErr.errorString() << ":"<<rawData;

    QJsonObject obj { doc.object() };
    member->leftRow = obj.value(member->KEY_LEFT_WHEEL_ROW).toDouble();
    member->rightRow = obj.value(member->KEY_RIGHT_WHEEL_ROW).toDouble();
}

double DriveModeDirect::getValue(int wheel)
{
    /*
     * Left row of wheels is number in 0,2,4... manner (even numbers)
     * Right row of wheels is number in 1,3,5... manner (odd numbers)
     */
    return isLeftRow(wheel) ? member->leftRow : member->rightRow;
}
