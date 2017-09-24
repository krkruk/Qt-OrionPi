#ifndef DRIVESETTINGS_H
#define DRIVESETTINGS_H
#include <QCoreApplication>
#include <QScopedPointer>


class DriveSettings
{
    class DriveSettingsPrivate;
    friend class DriveSettingsPrivate;

    DriveSettings(const DriveSettings &) = delete;
    DriveSettings(DriveSettings &&) = delete;
    DriveSettings &operator=(const DriveSettings &) = delete;
    DriveSettings &operator=(DriveSettings &&) = delete;
    DriveSettings();


public:
    ~DriveSettings();
    static DriveSettings *instance();

    QString getKeyGroup() const;
    QString getKeyAngularVelocity() const;
    QString getKeyCurrent() const;
    QString getKeyHeatSinkTemperature() const;
    QString getKeyPwm() const;
    QString getKeyErrorCode() const;

    QString getDriveMode(const char specifiedModeKey[]) const;


private:
    void load_data();

    QScopedPointer<DriveSettingsPrivate> member;
};

#endif // DRIVESETTINGS_H
