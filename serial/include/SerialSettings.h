#ifndef SERIALSETTINGS_H
#define SERIALSETTINGS_H
#include "../GlobalConstants.h"
#include "SerialConstants.h"
#include <QScopedPointer>


class SerialSettings
{
    class SettingsVariables;
    SerialSettings();
    SerialSettings(const SerialSettings&) = delete;
    SerialSettings(SerialSettings&&) = delete;
    SerialSettings &operator=(const SerialSettings &) = delete;
    SerialSettings &operator=(SerialSettings &&) = delete;

    void load_data();
public:
    static SerialSettings *instance();
    ~SerialSettings();

    QString getId() const;
    int getBaudrate() const;
    int getDataBits() const;
    int getFlowControl() const;
    int getParity() const;
    int getStopBits() const;
    int getWaitForReadLineMs() const;
    int getAttemptsForReadLine() const;

private:
    QScopedPointer<SettingsVariables> vars;
};

#endif // SERIALSETTINGS_H
