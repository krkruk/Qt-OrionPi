#include "DriveSettings.h"
#include "GlobalConstants.h"
#include "Orion/Drive/DriveConstants.h"
#include <QSettings>
#include <cstring>

using namespace CONST;

static void store_settings(QSettings &settings, const QString &key, const QVariant &value)
{
    if( !settings.contains(key) )
        settings.setValue(key, value);
}


class DriveSettings::DriveSettingsPrivate
{
    friend class DriveSettings;
public:
    DriveSettingsPrivate(){}

private:
    QString keyGroupName;
    QString keyAngularVelocity;
    QString keyCurrent;
    QString keyHeatSinkTemperature;
    QString keyPwm;
    QString keyErrorCode;

    QString keyDirectModeLeftWheelRow;
    QString keyDirectModeRightWheelRow;
};

DriveSettings::DriveSettings()
    : member(new DriveSettingsPrivate)
{    
    load_data();
}

DriveSettings::~DriveSettings()
{
    QSettings settings(ORGANIZATION, APP_NAME);
    settings.beginGroup(SETTINGS::DRIVE::GROUP_NAME);

    store_settings(settings, SETTINGS::DRIVE::KEY::DRIVE_GROUP_TAG, member->keyGroupName);
    store_settings(settings, SETTINGS::DRIVE::KEY::ANGULAR_VELOCITY, member->keyAngularVelocity);
    store_settings(settings, SETTINGS::DRIVE::KEY::CURRENT, member->keyCurrent);
    store_settings(settings, SETTINGS::DRIVE::KEY::HEATSINK_TEMP, member->keyHeatSinkTemperature);
    store_settings(settings, SETTINGS::DRIVE::KEY::PWM, member->keyPwm);
    store_settings(settings, SETTINGS::DRIVE::KEY::ERROR_CODE, member->keyErrorCode);

    settings.endGroup();

    settings.beginGroup(SETTINGS::DRIVE::MODE::GROUP_NAME);
    store_settings(settings, SETTINGS::DRIVE::MODE::DIRECT::KEY::LEFT_WHEEL_ROW,
                   SETTINGS::DRIVE::MODE::DIRECT::DEFAULT::LEFT_WHEEL_ROW_KEY);
    store_settings(settings, SETTINGS::DRIVE::MODE::DIRECT::KEY::RIGHT_WHEEL_ROW,
                   SETTINGS::DRIVE::MODE::DIRECT::DEFAULT::RIGHT_WHEEL_ROW_KEY);
    settings.endGroup();
}

DriveSettings *DriveSettings::instance()
{
    static DriveSettings s;
    return &s;
}

QString DriveSettings::getKeyGroup() const
{
    return member->keyGroupName;
}

QString DriveSettings::getKeyAngularVelocity() const
{
    return member->keyAngularVelocity;
}

QString DriveSettings::getKeyCurrent() const
{
    return member->keyCurrent;
}

QString DriveSettings::getKeyHeatSinkTemperature() const
{
    return member->keyHeatSinkTemperature;
}

QString DriveSettings::getKeyPwm() const
{
    return member->keyPwm;
}

QString DriveSettings::getKeyErrorCode() const
{
    return member->keyErrorCode;
}

QString DriveSettings::getDriveMode(const char specifiedModeKey[]) const
{
    auto isMode = [&](const char comparable[]) {
        return strcmp(specifiedModeKey, comparable) == 0;
    };

    if( isMode(SETTINGS::DRIVE::MODE::DIRECT::KEY::LEFT_WHEEL_ROW) )
        return member->keyDirectModeLeftWheelRow;
    if( isMode(SETTINGS::DRIVE::MODE::DIRECT::KEY::RIGHT_WHEEL_ROW) )
        return member->keyDirectModeRightWheelRow;

    return "";
}

void DriveSettings::load_data()
{
    QSettings settings(ORGANIZATION, APP_NAME);
    settings.beginGroup(SETTINGS::DRIVE::GROUP_NAME);

    member->keyGroupName = settings.value(SETTINGS::DRIVE::KEY::DRIVE_GROUP_TAG,
                                                SETTINGS::DRIVE::DEFAULT::DRIVE_GROUP_KEY).toString();

    member->keyAngularVelocity = settings.value(SETTINGS::DRIVE::KEY::ANGULAR_VELOCITY,
                                                SETTINGS::DRIVE::DEFAULT::ANGULAR_VELOCITY_KEY).toString();
    member->keyCurrent = settings.value(SETTINGS::DRIVE::KEY::CURRENT,
                                        SETTINGS::DRIVE::DEFAULT::CURRENT_KEY).toString();
    member->keyHeatSinkTemperature = settings.value(SETTINGS::DRIVE::KEY::HEATSINK_TEMP,
                                                    SETTINGS::DRIVE::DEFAULT::HEATSINK_TEMP_KEY).toString();
    member->keyPwm = settings.value(SETTINGS::DRIVE::KEY::PWM,
                                    SETTINGS::DRIVE::DEFAULT::PWM_KEY).toString();
    member->keyErrorCode = settings.value(SETTINGS::DRIVE::KEY::ERROR_CODE,
                                          SETTINGS::DRIVE::DEFAULT::ERROR_CODE_KEY).toString();
    settings.endGroup();

    settings.beginGroup(SETTINGS::DRIVE::MODE::GROUP_NAME);
    member->keyDirectModeLeftWheelRow = settings.value(SETTINGS::DRIVE::MODE::DIRECT::KEY::LEFT_WHEEL_ROW,
                                                       SETTINGS::DRIVE::MODE::DIRECT::DEFAULT::LEFT_WHEEL_ROW_KEY).toString();
    member->keyDirectModeRightWheelRow = settings.value(SETTINGS::DRIVE::MODE::DIRECT::KEY::RIGHT_WHEEL_ROW,
                                                       SETTINGS::DRIVE::MODE::DIRECT::DEFAULT::RIGHT_WHEEL_ROW_KEY).toString();
    settings.endGroup();
}

