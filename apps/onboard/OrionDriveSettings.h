#ifndef ORIONDRIVESETTINGS_H
#define ORIONDRIVESETTINGS_H
#include <QCoreApplication>
#include <QScopedPointer>


class OrionDriveSettings
{
    class OrionDriveSettingsPrivate;
    friend class OrionDriveSettingsPrivate;

    OrionDriveSettings(const OrionDriveSettings &) = delete;
    OrionDriveSettings(OrionDriveSettings &&) = delete;
    OrionDriveSettings &operator=(const OrionDriveSettings &) = delete;
    OrionDriveSettings &operator=(OrionDriveSettings &&) = delete;
    OrionDriveSettings();


public:
    ~OrionDriveSettings();
    static OrionDriveSettings *instance();

    QString getKeyGroup() const;
    QString getKeyAngularVelocity() const;
    QString getKeyCurrent() const;
    QString getKeyHeatSinkTemperature() const;
    QString getKeyPwm() const;
    QString getKeyErrorCode() const;

    QString getDriveMode(const char specifiedModeKey[]) const;


private:
    void load_data();

    QScopedPointer<OrionDriveSettingsPrivate> member;
};

#endif // ORIONDRIVESETTINGS_H
