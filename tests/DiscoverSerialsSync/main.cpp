#include <QCoreApplication>
#include <QSharedPointer>
#include <QSerialPortInfo>
#include <QDebug>
#include <QList>
#include "include/DeviceFinder.h"
#include "include/JsonDeviceFactory.h"
#include "include/interface/IfceDevice.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    (void)a;

    DeviceFinder finder(new JsonDeviceFactory);
    QList<QSharedPointer<IfceDevice>> devices = finder.discover();

    for( const QSharedPointer<IfceDevice> &device : devices )
        qDebug() << "Device discovered:" << device->getId() << device->getPortName();


    return 0;
}
