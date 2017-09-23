#ifndef ASYNCDEVICEFINDER_H
#define ASYNCDEVICEFINDER_H
#include <QObject>
#include <QSharedPointer>
#include <QSerialPort>
#include <QPointer>
#include <QList>
#include <QHash>

#include "interface/IfceSerialFinder.h"
#include "interface/IfceDeviceFactory.h"
#include "DeviceFinder.h"


class AsyncDeviceFinder : public IfceSerialFinder
{
    Q_OBJECT
public:
    explicit AsyncDeviceFinder(IfceDeviceFactory *deviceFactory, QObject *parent = 0);
    ~AsyncDeviceFinder();

    QList<QSharedPointer<IfceDevice> > discover() override;
    void setSerialSettings(QSharedPointer<IfceSerialSettings> settings) override;


signals:
    void signalDeviceFound(QSharedPointer<IfceDevice> device);
    void signalDiscoveryFinished();


private slots:
    void onReadyRead();
    void onErrorOccurred(QSerialPort::SerialPortError error);


private:
    void prepare_serial(const QSerialPortInfo &serialInfo);
    void action_parsing_error(QSharedPointer<QSerialPort> serial);
    void unregister_port(const QString &portName);
    void check_discovery_finished();

    static int MAX_READLINE_ATTEMPT;
    QScopedPointer<IfceDeviceFactory> deviceFactory;
    QHash<QString, QSerialPortInfo> availableSerials;
    QHash<QString, QSharedPointer<QSerialPort>> serials;
    QList<QString> availablePortNames;
    QSharedPointer<IfceSerialSettings> settings;
};

#endif // ASYNCDEVICEFINDER_H
