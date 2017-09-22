#ifndef MOCKDEVICEFINDER_H
#define MOCKDEVICEFINDER_H
#include "include/IfceSerialFinder.h"
#include "include/IfceDeviceFactory.h"
#include "exceptions/ParsingException.h"
#include "include/DefaultSerialSettings.h"

class MockDeviceFinder : public IfceSerialFinder
{
    QScopedPointer<IfceDeviceFactory> factory;
    QList<QByteArray> parsingData;
public:
    MockDeviceFinder(IfceDeviceFactory *factory)
        : factory{factory}
    {}

    MockDeviceFinder(IfceDeviceFactory *factory, const QList<QByteArray> &parsingData)
        : factory{factory}, parsingData{ parsingData }
    {}

    ~MockDeviceFinder() {}
    QList<QSharedPointer<IfceDevice>> discover() override {
        if( parsingData.isEmpty() )
            return generate_three_serial_devices();

        return generate_serial_devices_based_on_input();
    }

private:
    QList<QSharedPointer<IfceDevice>> generate_three_serial_devices()
    {
        QList<QSharedPointer<IfceDevice>> data;
        auto settings = QSharedPointer<DefaultSerialSettings>::create();

        for(auto i{0}; i < 3; i++)
            data.append(QSharedPointer<IfceDevice>{ factory->create(i, QSerialPortInfo(), settings) });

        return data;
    }

    QList<QSharedPointer<IfceDevice>>  generate_serial_devices_based_on_input()
    {
        QList<QSharedPointer<IfceDevice>> data;
        auto settings = QSharedPointer<DefaultSerialSettings>::create();
        for( const QByteArray &text : parsingData )
        {
            try {
                data.append( QSharedPointer<IfceDevice>{ factory->create(text, QSerialPortInfo(), settings)} );
            }catch(ParsingException &e) {
                Q_UNUSED(e)
            }
        }

        return data;
    }

public:
    void setSerialSettings(QSharedPointer<IfceSerialSettings> settings) override;
};

void MockDeviceFinder::setSerialSettings(QSharedPointer<IfceSerialSettings> settings)
{
    Q_UNUSED(settings)
}

#endif // MOCKDEVICEFINDER_H
