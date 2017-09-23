#ifndef THREADEDDEVICEFINDER_H
#define THREADEDDEVICEFINDER_H
#include "DeviceFinder.h"

/**
 * @brief The ThreadedDeviceFinder class is a threaded device finder
 *
 * An object of ThreadedDeviceFinder allows finding serial devices
 * that hold ID parameter in a threaded way.
 */
class ThreadedDeviceFinder : public DeviceFinder
{
    Q_OBJECT

public:
    explicit ThreadedDeviceFinder(IfceDeviceFactory *factory, QObject *parent = nullptr);
    ~ThreadedDeviceFinder();

    /**
     * @brief discover Discover devices
     * @return List of discovered devices. The information returned in the list
     * can be utilized in establishing permanent connections to serial devices.
     */
    QList<QSharedPointer<IfceDevice> > discover() override;
};

#endif // THREADEDDEVICEFINDER_H
