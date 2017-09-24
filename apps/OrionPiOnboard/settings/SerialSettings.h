#ifndef SERIALSETTINGS_H
#define SERIALSETTINGS_H
#include <QScopedPointer>
#include "include/interface/IfceSerialSettings.h"


class SerialSettings : public IfceSerialSettings
{
    class SettingsVariables;
    SerialSettings(const SerialSettings&) = delete;
    SerialSettings(SerialSettings&&) = delete;
    SerialSettings &operator=(const SerialSettings &) = delete;
    SerialSettings &operator=(SerialSettings &&) = delete;

    void load_data();
public:
    SerialSettings();
    ~SerialSettings();

    QString getId() const override;
    int getBaudrate() const override;
    int getDataBits() const override;
    int getFlowControl() const override;
    int getParity() const override;
    int getStopBits() const override;
    int getWaitForReadLineMs() const override;
    int getAttemptsForReadLine() const override;

private:
    QScopedPointer<SettingsVariables> vars;
};

#endif // SERIALSETTINGS_H
