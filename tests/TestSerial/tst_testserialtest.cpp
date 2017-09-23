#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QSerialPortInfo>
#include <QDebug>
#include <QVariant>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QEventLoop>
#include <QSignalSpy>
#include <QTimer>

#include "include/JsonDevice.h"
#include "include/DeviceFinder.h"
#include "include/JsonDeviceFactory.h"
#include "include/JsonSerialParser.h"
#include "include/SerialManager.h"
#include "include/interface/IfceSerialModel.h"
#include "include/SerialController.h"
#include "include/ThreadedDeviceFinder.h"
#include "include/AsyncDeviceFinder.h"
#include "include/SerialFactory.h"

#include "MockDeviceFinder.h"
#include "MockFoundDevice.h"
#include "MockSerialModel.h"

/**
 * @brief ENABLE_IO_TEST enables IO tests
 * In order to disable hardware, IO based tests type false.
 */
constexpr bool ENABLE_IO_TEST = true;
constexpr bool ENABLE_EVENT_LOOP_TEST = true;
constexpr bool ENABLE_DEVICE_TEST = true;

class TestSerialTest : public QObject
{
    Q_OBJECT

    const int mockId {1};
    const QString mockIdTag = "ID";
    const int mockValue {20};
    const QString mockPortName = "mockedPort";
    const QByteArray mockSerialData = QString("{\"ID\":%1,\"ROT\":%2}\r\n")
            .arg(mockId)
            .arg(mockValue)
            .toLatin1();

    QList<int> resultIDs;
    QList<QString> resultConnectedDevices;
    QList<QSharedPointer<IfceDevice>> mockDevices;
    QSharedPointer<DefaultSerialSettings> defaultSettings = QSharedPointer<DefaultSerialSettings>::create();

public:
    TestSerialTest() {}

    void runEventLoop(int millis)
    {
        QEventLoop loop;
        QTimer::singleShot(millis, &loop, SLOT(quit()));
        loop.exec();
    }


private Q_SLOTS:
    void initTestCase();
    void test_parse_json_id_serial_json_parser();
    void test_parse_invalid_json_id_serial_json_parser();
    void test_parsing_of_single_bin_line_with_id();
    void test_parsing_of_single_bin_line_with_id_trash_char_at_end();
    void test_parsing_of_single_bin_line_with_id_no_init_brace();
    void test_discover_three_mocked_devices_by_id();
    void test_discover_invalid_data_received();
    void test_create_serial_instance_hard_delete_mock_data();
    void test_real_serial_device_with_id_one();
    void test_real_serial_device_with_id_one_threaded_finder();
    void test_create_serial_manager_based_on_discovered_devices();
    void test_start_device_not_created();
    void test_test_controller_data_sent_by_manager();
    void test_model_parsing_json_data();
    void test_communication_between_controller_and_model();
    void test_communication_between_model_and_view();
    void test_communication_full_mvc_stack();
    void test_async_device_search();
    void test_serial_to_pc_comm();
};

void TestSerialTest::initTestCase()
{
    resultIDs << 0 << 1 << 2;
    resultConnectedDevices << mockPortName;
    mockDevices << QSharedPointer<MockFoundDevice>::create(mockId, mockPortName, defaultSettings);
}

void TestSerialTest::test_parse_json_id_serial_json_parser()
{
    const QByteArray line(mockSerialData);
    JsonSerialParser parser(mockIdTag);
    parser.parse(line);
    QVERIFY(parser.getId());
}

void TestSerialTest::test_parse_invalid_json_id_serial_json_parser()
{
    const QByteArray line(mockSerialData + "{\"\r\n");
    JsonSerialParser parser(mockIdTag);
    QVERIFY_EXCEPTION_THROWN(parser.parse(line), ParsingException);
}


void TestSerialTest::test_parsing_of_single_bin_line_with_id()
{
    QByteArray line(mockSerialData);
    QSharedPointer<IfceSerialSettings> settings = QSharedPointer<DefaultSerialSettings>::create();
    QScopedPointer<IfceDevice> device { new JsonDevice(settings, QSerialPortInfo()) };
    device->parse(line);

    QVERIFY(device->getId() == 1);
}

void TestSerialTest::test_parsing_of_single_bin_line_with_id_trash_char_at_end()
{
    QByteArray line(mockSerialData + "{\"\r\n");
    QSharedPointer<IfceSerialSettings> settings = QSharedPointer<DefaultSerialSettings>::create();
    QScopedPointer<IfceDevice> device { new JsonDevice(settings, QSerialPortInfo()) };
    QVERIFY_EXCEPTION_THROWN( device->parse(line), ParsingException);

}

void TestSerialTest::test_parsing_of_single_bin_line_with_id_no_init_brace()
{
    QByteArray line("\"ID\":1}\r\n");
    QSharedPointer<IfceSerialSettings> settings = QSharedPointer<DefaultSerialSettings>::create();
    QScopedPointer<IfceDevice> device { new JsonDevice(settings, QSerialPortInfo()) };
    QVERIFY_EXCEPTION_THROWN( device->parse(line), ParsingException);
}

void TestSerialTest::test_discover_three_mocked_devices_by_id()
{    
    QScopedPointer<IfceSerialFinder> discoverer { new MockDeviceFinder(new JsonDeviceFactory()) };
    QList<QSharedPointer<IfceDevice>> devices { discoverer->discover() };

    QList<int> discoveredIDs;
    for( const auto &device : devices )
        discoveredIDs.append(device->getId());

    QCOMPARE(discoveredIDs, resultIDs);
}

void TestSerialTest::test_discover_invalid_data_received()
{
    QList<QByteArray> inputData;
    inputData << QByteArray("{\"ID\":1}{\"\r\n")
              << QByteArray("\"ID\":1}\r\n");

    QScopedPointer<IfceSerialFinder> discoverer { new MockDeviceFinder{new JsonDeviceFactory, inputData} };
    QList<QSharedPointer<IfceDevice>> devices { discoverer->discover() };

    QVERIFY( devices.isEmpty() );
}

void TestSerialTest::test_create_serial_instance_hard_delete_mock_data()
{
    SerialFactory serialFactory;
    auto serial { serialFactory.create(QSerialPortInfo()) };
    QVERIFY( !serial.isNull() );
}

void TestSerialTest::test_real_serial_device_with_id_one()
{
    if( !ENABLE_IO_TEST ) QSKIP("Disabled IO test");
    QScopedPointer<IfceSerialFinder> discoverer { new DeviceFinder(new JsonDeviceFactory) };
    connect(dynamic_cast<DeviceFinder*>(discoverer.data()), &DeviceFinder::signalSerialError,
            this, [&](const QString &err){ qDebug() << "ERROR"  << err; });
    auto devices { discoverer->discover() };
    if( devices.isEmpty() ) QSKIP( "No device found" );

    bool hasMockId = false;
    for( const auto &device : devices )
        hasMockId |= device->getId() == mockId;

    QVERIFY( hasMockId );
}

void TestSerialTest::test_real_serial_device_with_id_one_threaded_finder()
{
    if( !ENABLE_IO_TEST ) QSKIP("Disabled IO test");
    QScopedPointer<IfceSerialFinder> discoverer { new ThreadedDeviceFinder(new JsonDeviceFactory) };
    connect(dynamic_cast<DeviceFinder*>(discoverer.data()), &DeviceFinder::signalSerialError,
            this, [&](const QString &err){ qDebug() << "ERROR"  << err; });
    auto devices { discoverer->discover() };
    if( devices.isEmpty() ) QSKIP( "No device found" );

    bool hasMockId = false;
    for( const auto &device : devices )
        hasMockId |= device->getId() == mockId;

    QVERIFY( hasMockId );
}

void TestSerialTest::test_create_serial_manager_based_on_discovered_devices()
{
    if( !ENABLE_IO_TEST ) QSKIP("Disabled IO test");
    QScopedPointer<IfceSerialFinder> discoverer { new DeviceFinder(new JsonDeviceFactory) };
    SerialManager manager;
    auto devices { discoverer->discover() };
    if( devices.isEmpty() ) QSKIP( "No device found" );

    manager.setDevices(devices);
    manager.start();

    QVERIFY( manager.hasSerial(mockId) );
}

void TestSerialTest::test_start_device_not_created()
{
    QScopedPointer<IfceSerialFinder> discoverer { new MockDeviceFinder(new JsonDeviceFactory) };
    SerialManager manager;
    auto devices { discoverer->discover() };

    manager.setDevices(devices);
    auto notConnected { manager.start() };
    const auto numberOfNotConnected { notConnected.length() };
    QVERIFY( numberOfNotConnected );
}

void TestSerialTest::test_test_controller_data_sent_by_manager()
{
    SerialManager manager;
    SerialController controller;
    manager.setController(&controller);

    manager.onLineReceived(mockId, mockSerialData);
    QCOMPARE( controller.getLastReceived(mockId), mockSerialData );
}

void TestSerialTest::test_model_parsing_json_data()
{
    QSharedPointer<IfceSerialModel> model { QSharedPointer<MockSerialModel>::create(mockId) };
    model->update(mockSerialData);
    QCOMPARE( qSharedPointerDynamicCast<MockSerialModel>(model)->
              getValue(), mockValue );
}

void TestSerialTest::test_communication_between_controller_and_model()
{
    SerialController controller;
    const auto modelId { mockId };
    QSharedPointer<IfceSerialModel> model { QSharedPointer<MockSerialModel>::create(modelId) };
    controller.addModel(model);
    controller.onLineReceived(modelId, mockSerialData);

    QCOMPARE( qSharedPointerDynamicCast<MockSerialModel>(model)->
              getValue(), mockValue );
}

void TestSerialTest::test_communication_between_model_and_view()
{
    if( !ENABLE_IO_TEST ) QSKIP("Disabled IO test");
    QScopedPointer<IfceSerialFinder> discoverer { new DeviceFinder(new JsonDeviceFactory) };
    SerialManager manager;
    auto devices { discoverer->discover() };
    if( devices.isEmpty() ) QSKIP( "No device found" );
    manager.setDevices(devices);
    manager.start();

    QSharedPointer<IfceSerialModel> model { QSharedPointer<MockSerialModel>::create(mockId) };
    model->addObserver(&manager);
    model->update(mockSerialData);
    model->notifyObservers();
    QCOMPARE( manager.getLastSent(mockId), mockSerialData );
}

void TestSerialTest::test_communication_full_mvc_stack()
{
    qDebug() << "MVC full stack started";
    if( !ENABLE_IO_TEST ) QSKIP("Disabled IO test");
    if( !ENABLE_EVENT_LOOP_TEST ) QSKIP("Disabled event loop test");
    QEventLoop loop;
    QScopedPointer<IfceSerialFinder> discoverer { new DeviceFinder(new JsonDeviceFactory) };
    SerialManager manager;
    connect(&manager, &SerialManager::serialError, this,
            [&](int id, const QString &errorString){
        qDebug() << QString("Error occured! DeviceId: %1 - error: %2").arg(id).arg(errorString);
    });

    SerialController controller;
    QSharedPointer<IfceSerialModel> model { QSharedPointer<MockSerialModel>::create(mockId) };
    manager.setController(&controller);
    controller.addModel(model);
    model->addObserver(&manager);
    auto devices { discoverer->discover() };
    if( devices.isEmpty() ) QSKIP( "No device found" );
    manager.setDevices(devices);

    auto devicesNotConnected { manager.start() };
    for( int deviceId : devicesNotConnected ) qDebug() << "Not connected. Id of the device:" << deviceId;

    QTimer::singleShot(5000, &loop, SLOT(quit()));
    loop.exec();

    QCOMPARE( qSharedPointerDynamicCast<MockSerialModel>(model)->
              getValue(), mockValue );
}

void TestSerialTest::test_async_device_search()
{
    if( !ENABLE_DEVICE_TEST ) QSKIP("SKIPPED 4 device test");

    constexpr int DEVICE_COUNT { 4 };
    if( !ENABLE_EVENT_LOOP_TEST ) QSKIP("Disabled event loop test");
    QScopedPointer<IfceSerialFinder> discoverer { new AsyncDeviceFinder(new JsonDeviceFactory) };
    QPointer<AsyncDeviceFinder> asyncFinder { dynamic_cast<AsyncDeviceFinder*>(discoverer.data()) };

    QList<QSharedPointer<IfceDevice>> devices;
    connect(asyncFinder.data(), &AsyncDeviceFinder::signalDeviceFound, this, [&](QSharedPointer<IfceDevice> device){
        devices.append(device);
        qDebug() << "Device #id: " << device->getId() << "found at port:" << device->getPortName();
    });
    connect(asyncFinder.data(), &AsyncDeviceFinder::signalSerialError, this, [&](QString error){
        qDebug() << "Serial error: "<< error;
    });
    discoverer->discover();
    runEventLoop(3000);
    QCOMPARE( devices.count(), DEVICE_COUNT );
}

void TestSerialTest::test_serial_to_pc_comm()
{
    if( !ENABLE_IO_TEST ) QSKIP("Disabled IO test");

    const QString toBeReceived {"{\"ID\":1,\"TEMP\":70,\"ROT\":20,\"CUR\":5}\r\n"};
    QScopedPointer<IfceSerialFinder> finder { new ThreadedDeviceFinder(new JsonDeviceFactory) };
    const auto devices { finder->discover() };
    QList<QSharedPointer<IfceDevice>> selectedDevice;
    for( const auto &device : devices )
        if( device->getId() == mockId ) selectedDevice.append(device);

    if( selectedDevice.isEmpty() ) QSKIP("No device present");

    SerialManager manager;
    SerialController controller;
    QSharedPointer<IfceSerialModel> model ( QSharedPointer<MockSerialModel>::create(mockId) );
    controller.addModel(model);
    manager.setDevices(selectedDevice);
    manager.setController(&controller);
    manager.start();
    runEventLoop(3000);
    const auto receivedData = qSharedPointerCast<MockSerialModel>(model)->getRawData();
    QCOMPARE( receivedData, toBeReceived.toLatin1());
}

QTEST_MAIN(TestSerialTest)

#include "tst_testserialtest.moc"
