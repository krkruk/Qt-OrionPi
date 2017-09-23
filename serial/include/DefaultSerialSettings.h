#ifndef DEFAULTSERIALSETTINGS_H
#define DEFAULTSERIALSETTINGS_H
#include "interface/IfceSerialSettings.h"

class DefaultSerialSettings : public IfceSerialSettings
{
public:
    ~DefaultSerialSettings() {}

    QString getId() const override { return "ID"; }
    int getBaudrate() const override { return 115200; }
    int getDataBits() const override { return 8; }
    int getFlowControl() const override { return 0; }
    int getParity() const override { return 0; }
    int getStopBits() const override { return 1; }
    int getWaitForReadLineMs() const override { return 1000; }
    int getAttemptsForReadLine() const override { return 5; }
};

#endif // DEFAULTSERIALSETTINGS_H
