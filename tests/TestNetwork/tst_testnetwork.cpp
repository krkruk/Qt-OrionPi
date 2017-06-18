#include <QString>
#include <QtTest>
#include <QDebug>
#include <QCoreApplication>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QHostAddress>
#include <QWeakPointer>
#include <QTimer>
#include <QEventLoop>
#include <QLocalServer>
#include <algorithm>
#include <typeinfo>


#include "IfceDispatchEngine.h"
#include "JsonDispatchEngine.h"

#include "IfceGlobalServer.h"
#include "IfceLocalSocket.h"
#include "TcpGlobalServer.h"
#include "TcpLocalSocket.h"

#include "IfceLocalServer.h"
#include "IfceIpcServer.h"
#include "TcpLocalServer.h"
#include "IpcServer.h"

#include "IfceIpcSocket.h"
#include "IpcSocket.h"

#include "MockRemoteSocket.h"
#include "MockObserver.h"
#include "Observable.h"
#include "Observer.h"

#include "RootNode.h"
#include "IfceServerFactory.h"
#include "TcpGlobalServerFactory.h"
#include "IfceSocketFactory.h"
#include "IfceSocketFactory.h"
#include "TcpLocalSocketFactory.h"


class TestNetwork : public QObject
{
    Q_OBJECT
    const QString driveData;
    const QString manipulatorData;
    const QByteArray remoteData;
    const QString tagDriveData = "drive";
    const QString tagManipulatorData = "manipulator";
    const QString serverName{ "pl.projektorion.localUberServer" };
public:
    TestNetwork();

    void runEventLoop(int millis) {
        QEventLoop loop;
        QTimer::singleShot(millis, &loop, SLOT(quit()));
        loop.exec();
    }

private Q_SLOTS:
    void initTestCase();
    void test_incoming_data_dispatching_engine();
    void test_incoming_data_dispatching_engine_stack();
    void test_outcoming_data_encode_enigne();
    void test_local_socket_interface();
    void test_communication_global_server_to_local_sockets();
    void test_registering_local_server_to_ipc_server();
    void test_communication_between_ipc_local_server_to_socket();
    void test_communication_between_local_ipc_socket_to_ipc_server();
    void test_communication_between_ipc_server_to_local_server();
    void test_communication_between_local_server_to_ipc_server();
    void test_communication_between_tcp_local_server_to_local_tcp_sockets();
    void test_communication_between_local_tcp_socket_to_local_tcp_server();
    void test_communication_between_local_sockets_to_global_server();
    void test_communication_between_global_server_to_local_socket();
    void test_communication_between_remote_socket_to_global_tcp_server();
    void test_communication_between_global_tcp_server_to_remote_socket();
    void test_communication_between_ipc_local_socket_to_processor_class();
    void test_communication_between_processor_class_and_ipc_local_socket();
    void test_global_server_factory();
    void test_local_socket_factory();
    void test_root_node_append_local_sockets();
};

TestNetwork::TestNetwork()
    : driveData(R"({"ang_vel":20,"radius":"infinity"})"),
      manipulatorData(R"({"grasp":50})"),
      remoteData(QString(R"({"drive":%1,"manipulator":%2})").arg(driveData).arg(manipulatorData).toLatin1() + "\r\n")
{
}

void TestNetwork::initTestCase()
{
    QLocalServer::removeServer(serverName);
}

void TestNetwork::test_incoming_data_dispatching_engine()
{
    QSharedPointer<IfceDispatchEngine> dispatcher( new JsonDispatchEngine );
    const auto DRIVE_DATA { dispatcher->addTag(tagDriveData) };
    const auto MANIPULATOR_DATA { dispatcher->addTag(tagManipulatorData) };

    QVERIFY( dispatcher->dispatch(remoteData) );
    const QByteArray drive = dispatcher->getDispatched(DRIVE_DATA);
    const QByteArray manipulator = dispatcher->getDispatched(MANIPULATOR_DATA);

    QCOMPARE( drive, driveData.toLatin1() );
    QCOMPARE( manipulator, manipulatorData.toLatin1() );
}

void TestNetwork::test_incoming_data_dispatching_engine_stack()
{
    auto dispatcher { JsonDispatchEngine() };
    const QStringList tags{tagDriveData, tagManipulatorData};
    dispatcher.addTag(tags);

    QVERIFY( dispatcher.dispatch(remoteData) );
    const QByteArray drive = dispatcher.getDispatched(tagDriveData);
    const QByteArray manipulator = dispatcher.getDispatched(tagManipulatorData);

    QCOMPARE( drive, driveData.toLatin1() );
    QCOMPARE( manipulator, manipulatorData.toLatin1() );
    auto dispatchedTags = dispatcher.getTags();
    std::sort(dispatchedTags.begin(), dispatchedTags.end(), std::less<QString>());
    QCOMPARE( dispatchedTags, tags );
}

void TestNetwork::test_outcoming_data_encode_enigne()
{
    JsonDispatchEngine dispatcher;
    dispatcher.encode(tagDriveData, driveData.toLatin1());
    dispatcher.encode(tagManipulatorData, manipulatorData.toLatin1());
    const auto result = dispatcher.getEncoded();
    QCOMPARE( result, remoteData.simplified() );
    QVERIFY( dispatcher.getEncoded().isEmpty() );
}

void TestNetwork::test_local_socket_interface()
{
    const QHostAddress address { QHostAddress::LocalHost };
    constexpr int port { 6000 };
    const QString tag { tagDriveData };
    QSharedPointer<IfceLocalSocket> drive( new TcpLocalSocket(address, port, tag, this) );
    drive->onRemoteMessageReceived(driveData.toLatin1());
    QCOMPARE( drive->getTag(), tag );
    QCOMPARE( drive->getLastRemoteMessage(), driveData.toLatin1() );
}

void TestNetwork::test_communication_global_server_to_local_sockets()
{
    QSharedPointer<IfceDispatchEngine> dispatcher( new JsonDispatchEngine );
    dispatcher->addTag(tagDriveData);

    QScopedPointer<IfceGlobalServer> net( new TcpGlobalServer(this) );
    net->setDispatcher(dispatcher);

    const QHostAddress address { QHostAddress::LocalHost };
    constexpr int port { 6000 };
    const QString tag { tagDriveData };
    QSharedPointer<IfceLocalSocket> drive( new TcpLocalSocket(address, port, tag, this) );
    net->addLocalObserver(drive);
    net->onRemoteMessageReceived(remoteData);

    QCOMPARE( net->getLastRemoteMessage(), remoteData );
    QCOMPARE( drive->getLastRemoteMessage(), driveData.toLatin1() );
}

void TestNetwork::test_registering_local_server_to_ipc_server()
{
    const auto maxConn { 2 };
    const QHostAddress address { "127.0.0.1" };
    constexpr int port { 7000 };

    QScopedPointer<IfceLocalServer> deviceServer( new TcpLocalServer(port, this) );
    deviceServer->setListenAddress(address);
    deviceServer->setMaxConnections(maxConn);
    QVERIFY( deviceServer->start() );

    QSharedPointer<IfceIpcServer> local( new IpcServer(serverName) );
    local->setServerName(serverName);
    local->setMaxConnections(maxConn);
    deviceServer->setIpcServer(local);
    QVERIFY( local->start() );

    deviceServer->onRemoteMessageReceived(driveData.toLatin1() );
    QCOMPARE( local->getLastRemoteMessage(), driveData.toLatin1() );
}

void TestNetwork::test_communication_between_ipc_local_server_to_socket()
{
    const QString serverName{ "pl.projektorion.localUberServer" };
    QSharedPointer<IfceIpcServer> ipcServer( new IpcServer(serverName) );
    ipcServer->start();

    QSharedPointer<IfceIpcSocket> ipcSocket( new IpcSocket(serverName) );
    runEventLoop(10);
    ipcServer->onRemoteMessageReceived(driveData.toLatin1());
    runEventLoop(10);
    auto recvd { ipcSocket->getLastRemoteMessage() };
    QCOMPARE( recvd, driveData.toLatin1() );
}

void TestNetwork::test_communication_between_local_ipc_socket_to_ipc_server()
{
    QSharedPointer<IfceIpcServer> ipcServer( new IpcServer(serverName) );
    ipcServer->start();

    QSharedPointer<IfceIpcSocket> ipcSocket( new IpcSocket(serverName) );
    runEventLoop(10);
    ipcSocket->onLocalMessageReceived(driveData.toLatin1());
    runEventLoop(10);
    auto recvd { ipcServer->getLastLocalMessage() };
    QCOMPARE( recvd, driveData.toLatin1() );
}

void TestNetwork::test_communication_between_ipc_server_to_local_server()
{
    const auto maxConn { 2 };
    const QHostAddress address { "127.0.0.1" };
    constexpr int port { 7000 };

    QScopedPointer<IfceLocalServer> deviceServer( new TcpLocalServer(port, this) );
    deviceServer->setListenAddress(address);
    deviceServer->setMaxConnections(maxConn);
    QVERIFY( deviceServer->start() );

    QSharedPointer<IfceIpcServer> local( new IpcServer(serverName) );
    deviceServer->setIpcServer(local);
    local->onLocalMessageReceived(driveData.toLatin1());
    runEventLoop(10);
    auto recvd { deviceServer->getLastLocalMessage() };
    QCOMPARE( recvd, driveData.toLatin1() );
}

void TestNetwork::test_communication_between_local_server_to_ipc_server()
{
    const auto maxConn { 2 };
    const QHostAddress address { "127.0.0.1" };
    constexpr int port { 7000 };

    QScopedPointer<IfceLocalServer> deviceServer( new TcpLocalServer(port, this) );
    deviceServer->setListenAddress(address);
    deviceServer->setMaxConnections(maxConn);
    QVERIFY( deviceServer->start() );

    QSharedPointer<IfceIpcServer> local( new IpcServer(serverName) );
    deviceServer->setIpcServer(local);
    deviceServer->onRemoteMessageReceived(driveData.toLatin1());
    runEventLoop(10);
    auto recvd { local->getLastRemoteMessage() };
    QCOMPARE( recvd, driveData.toLatin1() );
}

void TestNetwork::test_communication_between_tcp_local_server_to_local_tcp_sockets()
{
    const auto maxConn { 2 };
    const QHostAddress address { "127.0.0.1" };
    constexpr int port { 7001 };

    QScopedPointer<IfceLocalSocket> driveSocket( new TcpLocalSocket(address, port, tagDriveData, this) );
    runEventLoop(200);
    QScopedPointer<IfceLocalServer> deviceServer( new TcpLocalServer(port, this) );
    deviceServer->setListenAddress(address);
    deviceServer->setMaxConnections(maxConn);
    QVERIFY( deviceServer->start() );
    runEventLoop(50);
    deviceServer->onLocalMessageReceived(driveData.toLatin1());
    runEventLoop(10);
    auto recvd { driveSocket->getLastLocalMessage() };
    QCOMPARE( recvd, driveData.toLatin1() );
}

void TestNetwork::test_communication_between_local_tcp_socket_to_local_tcp_server()
{
    const auto maxConn { 2 };
    const QHostAddress address { "127.0.0.1" };
    constexpr int port { 7001 };

    QScopedPointer<IfceLocalSocket> driveSocket( new TcpLocalSocket(address, port, tagDriveData, this) );
    runEventLoop(200);
    QScopedPointer<IfceLocalServer> deviceServer( new TcpLocalServer(port, this) );
    deviceServer->setListenAddress(address);
    deviceServer->setMaxConnections(maxConn);
    QVERIFY( deviceServer->start() );
    runEventLoop(50);
    driveSocket->onRemoteMessageReceived(driveData.toLatin1());
    runEventLoop(10);
    const auto recvd { deviceServer->getLastRemoteMessage() };
    QCOMPARE( recvd, driveData.toLatin1() );
}

void TestNetwork::test_communication_between_local_sockets_to_global_server()
{
    const QHostAddress address { "127.0.0.1" };
    constexpr int port { 7001 };

    QScopedPointer<IfceGlobalServer> globalServer( new TcpGlobalServer(this) );
    QSharedPointer<IfceLocalSocket> driveSocket( new TcpLocalSocket(address, port, tagDriveData, this) );
    globalServer->setDispatcher(QSharedPointer<JsonDispatchEngine>::create());
    globalServer->addLocalObserver(driveSocket);
    driveSocket->onLocalMessageReceived(driveData.toLatin1());
    runEventLoop(10);
    const auto recvd { globalServer->getLastLocalMessage() };
    JsonDispatchEngine e;
    e.encode(tagDriveData, driveData.toLocal8Bit());
    QCOMPARE( recvd, e.getEncoded() );
}

void TestNetwork::test_communication_between_global_server_to_local_socket()
{
    const QHostAddress address { "127.0.0.1" };
    constexpr int port { 7001 };

    QScopedPointer<IfceGlobalServer> globalServer( new TcpGlobalServer(this) );
    QSharedPointer<IfceLocalSocket> driveSocket( new TcpLocalSocket(address, port, tagDriveData, this) );
    auto dispatcher { QSharedPointer<JsonDispatchEngine>::create() };
    dispatcher->addTag({tagDriveData, tagManipulatorData});
    globalServer->setDispatcher(dispatcher);
    globalServer->addLocalObserver(driveSocket);
    globalServer->onRemoteMessageReceived(remoteData);
    const auto recvd { driveSocket->getLastRemoteMessage() };
    QCOMPARE( recvd, driveData.toLatin1() );
}

void TestNetwork::test_communication_between_remote_socket_to_global_tcp_server()
{
    const QHostAddress address { "127.0.0.1" };
    constexpr int port { 7002 };
    MockRemoteSocket socket(address, port, this);
    QScopedPointer<IfceGlobalServer> globalServer( new TcpGlobalServer(address, port, this) );
    globalServer->setDispatcher(QSharedPointer<JsonDispatchEngine>::create());
    globalServer->start();
    runEventLoop(50);
    socket.onSendDataToServer(remoteData);
    runEventLoop(10);
    const auto result { globalServer->getLastRemoteMessage() };
    QCOMPARE( result, remoteData );
}

void TestNetwork::test_communication_between_global_tcp_server_to_remote_socket()
{
    const QHostAddress address { "127.0.0.1" };
    constexpr int port { 7002 };
    constexpr int drivePort { 8000 };
    MockRemoteSocket socket(address, port, this);
    QScopedPointer<IfceGlobalServer> globalServer( new TcpGlobalServer(address, port, this) );
    QSharedPointer<IfceLocalSocket> driveSocket( new TcpLocalSocket(address, drivePort, tagDriveData, this) );
    globalServer->setDispatcher(QSharedPointer<JsonDispatchEngine>::create());
    globalServer->addLocalObserver(driveSocket);
    globalServer->start();
    runEventLoop(50);
    driveSocket->onLocalMessageReceived(driveData.toLocal8Bit());
    runEventLoop(10);
    const auto result { socket.getReceivedData() };
    JsonDispatchEngine e;
    e.encode(tagDriveData, driveData.toLocal8Bit());
    QCOMPARE( result, e.getEncoded() );
}

void TestNetwork::test_communication_between_ipc_local_socket_to_processor_class()
{
    QSharedPointer<Observer> observer( new MockObserver() );
    QScopedPointer<IfceIpcSocket> socket( new IpcSocket(serverName, this) );
    socket->addObserver(observer);
    runEventLoop(50);
    QScopedPointer<IfceIpcServer> server( new IpcServer(serverName, this) );
    QVERIFY( server->start() );
    runEventLoop(200);
    server->onRemoteMessageReceived(driveData.toLatin1());
    runEventLoop(20);
    const auto result { qSharedPointerCast<MockObserver>(observer)->getLastRemoteMessage() };
    QCOMPARE( result, socket->getLastRemoteMessage() );
    QCOMPARE( result, driveData.toLatin1() );
}

void TestNetwork::test_communication_between_processor_class_and_ipc_local_socket()
{
    QScopedPointer<IfceIpcServer> server( new IpcServer(serverName, this) );
    QScopedPointer<IfceIpcSocket> socket ( new IpcSocket(serverName, this) );
    QSharedPointer<Observer> observer( new MockObserver() );
    socket->addObserver(observer);
    server->start();
    runEventLoop(120);
    emit observer->signalLocalResponse(driveData.toLatin1());
    runEventLoop(10);
    const auto result { server->getLastLocalMessage() };
    QCOMPARE( result, driveData.toLatin1() );
}

void TestNetwork::test_global_server_factory()
{
    const QHostAddress globalAddress { QHostAddress::LocalHost };
    constexpr auto globalPort {3000};
    const QString serverName { "myServer" };

    QSharedPointer<IfceServerFactory> globalServerFactory { QSharedPointer<TcpGlobalServerFactory>::create() };
    globalServerFactory->setAddress(globalAddress);
    globalServerFactory->setPort(globalPort);
    globalServerFactory->setServerName(serverName);
    auto server = globalServerFactory->create();
    QVERIFY( qSharedPointerDynamicCast<IfceGlobalServer>(server) );
}

void TestNetwork::test_local_socket_factory()
{
    const QHostAddress localAddress { QHostAddress::LocalHost };
    constexpr auto localPort {3000};
    const QString serverName { "myServer" };
    QSharedPointer<IfceLocalSocketFactory> localSocketFactory { QSharedPointer<TcpLocalSocketFactory>::create() };
    localSocketFactory->setAddress(localAddress);
    localSocketFactory->setPort(localPort);
    localSocketFactory->setServerName(serverName);
    localSocketFactory->setTag(tagDriveData);
    auto instance { localSocketFactory->create() };
    QVERIFY( qSharedPointerDynamicCast<TcpLocalSocket>(instance) );
    QCOMPARE( qSharedPointerDynamicCast<TcpLocalSocket>(instance)->getTag(), tagDriveData );
}

void TestNetwork::test_root_node_append_local_sockets()
{
    const QHostAddress globalAddress { QHostAddress::LocalHost };
    constexpr auto globalPort {3000};
    const QHostAddress driveAddress { QHostAddress::LocalHost };
    const auto drivePort { 5000 };
    QScopedPointer<RootNode> rootNode{ new RootNode(globalAddress, globalPort, this) };
    QSharedPointer<IfceLocalSocketFactory> socketFactory { QSharedPointer<TcpLocalSocketFactory>::create() };
    QSharedPointer<IfceServerFactory> globalServerFactory { QSharedPointer<TcpGlobalServerFactory>::create() };
    QSharedPointer<IfceDispatchEngine> dispatcher { QSharedPointer<JsonDispatchEngine>::create() };
    socketFactory->setAddress(driveAddress);
    socketFactory->setPort(drivePort);

    rootNode->setDispatcher(dispatcher);
    rootNode->setSocketFactory(socketFactory);
    rootNode->addSocket(driveAddress, drivePort, tagDriveData);
    rootNode->setGlobalServerFactory(globalServerFactory);
    QVERIFY( rootNode->start() );
    runEventLoop(10);
    QTcpSocket socket(this);
    socket.connectToHost(globalAddress, globalPort);
    socket.waitForConnected();
    socket.write(remoteData);
    runEventLoop(10);

    QWeakPointer<IfceLocalSocket> st { rootNode->getSocket(tagDriveData) };
    QByteArray recvd;
    if( auto s = st.toStrongRef() )
        recvd = s->getLastRemoteMessage();
    QCOMPARE( recvd, driveData.toLatin1() );
}

QTEST_MAIN(TestNetwork)

#include "tst_testnetwork.moc"
