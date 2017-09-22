#include "ThreadedDeviceFinder.h"
#include <QFuture>


ThreadedDeviceFinder::ThreadedDeviceFinder(IfceDeviceFactory *factory, QObject *parent)
    : DeviceFinder(factory, parent)
{

}

ThreadedDeviceFinder::~ThreadedDeviceFinder()
{
}

QList<QSharedPointer<IfceDevice> > ThreadedDeviceFinder::discover()
{
    QList<QFuture<QSharedPointer<IfceDevice>>> futures;
    const auto availablePorts { QSerialPortInfo::availablePorts() };
    for( const auto &availablePort : availablePorts )
        futures.append( QtConcurrent::run([&](const QSerialPortInfo &portInfo) {
                return DeviceFinder::run_serial(portInfo);
            }, availablePort)
        );

    QList<QSharedPointer<IfceDevice>> devices;
    for( auto future : futures ) {
        future.waitForFinished();
        if( auto &&device = future.result() )
            devices.append(device);
    }

    return devices;
}
