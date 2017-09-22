#include "SerialSettings.h"
#include <QSettings>


using namespace CONST::SETTINGS;


class SerialSettings::SettingsVariables
{
    friend class SerialSettings;

private:
    QString id;
    int baudrate;
    int dataBits;
    int flowControl;
    int parity;
    int stopBits;
    int waitForReadLineMs;
    int attemptsReadLine;
};

static void store_settings(QSettings &settings, const QString &key, const QVariant &value)
{
    if( !settings.contains(key) )
        settings.setValue(key, value);
}

SerialSettings::SerialSettings()
    : vars{ new SerialSettings::SettingsVariables }
{
    load_data();
}

void SerialSettings::load_data()
{
    QSettings settings(CONST::ORGANIZATION, CONST::APP_NAME);
    settings.beginGroup(CONST::SETTINGS::SERIAL::GROUP_NAME);

    vars->id = settings.value(SERIAL::KEYS::ID, SERIAL::DEFAULT::ID_KEY).toString();
    vars->waitForReadLineMs = settings.value(SERIAL::KEYS::WAIT_FOR_READLINE_MS,
                                             SERIAL::DEFAULT::WAIT_FOR_READLINE_MS).toInt();
    vars->baudrate = settings.value(SERIAL::KEYS::BAUDRATE, SERIAL::DEFAULT::BAUDRATE).toInt();
    vars->dataBits = settings.value(SERIAL::KEYS::DATA_BITS, SERIAL::DEFAULT::DATABITS).toInt();
    vars->flowControl = settings.value(SERIAL::KEYS::FLOW_CONTROL,
                                       SERIAL::DEFAULT::FLOW_CONTROL).toInt();
    vars->parity = settings.value(SERIAL::KEYS::PARITY, SERIAL::DEFAULT::PARITY).toInt();
    vars->stopBits= settings.value(SERIAL::KEYS::STOP_BITS, SERIAL::DEFAULT::STOP_BITS).toInt();
    vars->attemptsReadLine = settings.value(SERIAL::KEYS::ATTEMPTS_READ_LINE,
                                            SERIAL::DEFAULT::ATTEMPTS_READ_LINE).toInt();

    settings.endGroup();
}

SerialSettings::~SerialSettings()
{
    QSettings settings(CONST::ORGANIZATION, CONST::APP_NAME);
    settings.beginGroup(CONST::SETTINGS::SERIAL::GROUP_NAME);

    store_settings(settings, SERIAL::KEYS::ID, vars->id);
    store_settings(settings, SERIAL::KEYS::WAIT_FOR_READLINE_MS, vars->waitForReadLineMs);
    store_settings(settings, SERIAL::KEYS::BAUDRATE, vars->baudrate);
    store_settings(settings, SERIAL::KEYS::DATA_BITS, vars->dataBits);
    store_settings(settings, SERIAL::KEYS::FLOW_CONTROL, vars->flowControl);
    store_settings(settings, SERIAL::KEYS::PARITY, vars->parity);
    store_settings(settings, SERIAL::KEYS::STOP_BITS, vars->stopBits);
    store_settings(settings, SERIAL::KEYS::ATTEMPTS_READ_LINE, vars->attemptsReadLine);

    settings.endGroup();
}

QString SerialSettings::getId() const
{
    return vars->id;
}

int SerialSettings::getBaudrate() const
{
    return vars->baudrate;
}

int SerialSettings::getDataBits() const
{
    return vars->dataBits;
}

int SerialSettings::getFlowControl() const
{
    return vars->flowControl;
}

int SerialSettings::getParity() const
{
    return vars->parity;
}

int SerialSettings::getStopBits() const
{
    return vars->stopBits;
}

int SerialSettings::getWaitForReadLineMs() const
{
    return vars->waitForReadLineMs;
}

int SerialSettings::getAttemptsForReadLine() const
{
    return vars->attemptsReadLine;
}
