#ifndef IFCESERIALSETTINGS_H
#define IFCESERIALSETTINGS_H

class QString;

class IfceSerialSettings
{
public:
    virtual ~IfceSerialSettings() {}

    virtual QString getId() const = 0;
    virtual int getBaudrate() const = 0;
    virtual int getDataBits() const = 0;
    virtual int getFlowControl() const = 0;
    virtual int getParity() const = 0;
    virtual int getStopBits() const = 0;
    virtual int getWaitForReadLineMs() const = 0;
    virtual int getAttemptsForReadLine() const = 0;
};

#endif // IFCESERIALSETTINGS_H
