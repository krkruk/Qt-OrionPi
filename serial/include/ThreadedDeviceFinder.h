#ifndef THREADEDDEVICEFINDER_H
#define THREADEDDEVICEFINDER_H

#include <QtConcurrent/QtConcurrent>

#include "DeviceFinder.h"


class ThreadedDeviceFinder : public DeviceFinder
{
    Q_OBJECT

public:
    explicit ThreadedDeviceFinder(IfceDeviceFactory *factory, QObject *parent = nullptr);
    ~ThreadedDeviceFinder();

    QList<QSharedPointer<IfceDevice> > discover() override;
};

#endif // THREADEDDEVICEFINDER_H
