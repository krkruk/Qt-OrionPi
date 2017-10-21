#include <QString>
#include <QtTest>
#include <QDebug>
#include <QCoreApplication>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QHostAddress>
#include <QWeakPointer>
#include <QTimer>
#include <QSignalSpy>
#include <QEventLoop>
#include <QLocalServer>
#include <algorithm>
#include <typeinfo>

#include "include/IfceServer.h"
#include "include/IfceIpSocket.h"
#include "TcpServer.h"
#include "TcpSocket.h"
#include "UdpServer.h"
#include "UdpSocket.h"

#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/message_differencer.h>
#include "proto/testMsg.pb.h"


namespace {
    constexpr bool ENABLE_TCP_TEST = true;
    constexpr bool ENABLE_UDP_TEST = true;
}

class TestNetwork : public QObject
{
    Q_OBJECT
    const QHostAddress serverAddress { QHostAddress::LocalHost };
    const QHostAddress socketAddress { QHostAddress::LocalHost };
    const int serverPort {5500};
    const int socketPort {5510};

    QSharedPointer<IfceServer> server;
    QSharedPointer<IfceServer> udpServer;
public:
    enum Socket {
        TCP_SOCKET,
        UDP_SOCKET
    };

    TestNetwork();

    void runEventLoop(int millis) {
        QEventLoop loop;
        QTimer::singleShot(millis, &loop, SLOT(quit()));
        loop.exec();
    }

    QSharedPointer<IfceIpSocket> createSocket(Socket socket=TCP_SOCKET) {
        QSharedPointer<IfceIpSocket> remoteSocket;
        switch( socket ) {
        case TCP_SOCKET: remoteSocket
                    = QSharedPointer<TcpSocket>::create(socketAddress, 0, this);
            break;
        case UDP_SOCKET: remoteSocket
                    = QSharedPointer<UdpSocket>::create(socketAddress, socketPort, this);
            break;
        default:
            break;
        }
        remoteSocket->connectToHost(serverAddress, serverPort);
        return remoteSocket;
    }

private Q_SLOTS:
    void initTestCase();
    void test_tcp_comm_socket_to_server();
    void test_tcp_comm_server_to_socket();
    void test_tcp_comm_send_2_pkgs_at_once_sock_to_serv();
    void test_tcp_comm_send_2_pkgs_at_once_serv_to_sock();
    void test_tcp_comm_socket_to_server_std_string();
    void test_tcp_comm_server_to_socket_std_string();
    void test_tcp_comm_socket_to_server_std_protobuf();
    void test_tcp_comm_server_to_socket_std_protobuf();
    void test_tcp_comm_reconnection_to_server();
    void test_udp_comm_socket_to_server();
    void test_udp_comm_server_to_socket();
    void test_udp_comm_send_2_pkgs_at_once_sock_to_serv();
    void test_udp_comm_send_2_pkgs_at_once_serv_to_sock();
    void test_udp_comm_socket_to_server_std_protobuf();
    void test_udp_comm_server_to_socket_std_protobuf();
};

TestNetwork::TestNetwork()
    : server{ QSharedPointer<TcpServer>::create(serverAddress, serverPort, this) },
      udpServer{ QSharedPointer<UdpServer>::create(serverAddress, serverPort, this) }
{
}

void TestNetwork::initTestCase()
{
}

void TestNetwork::test_tcp_comm_socket_to_server()
{
    if( !ENABLE_TCP_TEST ) QSKIP("TCP test skipped");
    QSharedPointer<IfceIpSocket> remoteSocket{ createSocket(TCP_SOCKET) };
    QSignalSpy spy(server.data(), SIGNAL(signalMessageReceived(QByteArray)));
    runEventLoop(1000);
    const QString data { "random send\n\n data \t\tHelloWorld" };
    remoteSocket->send(data);
    runEventLoop(20);

    const std::string lastReceived { server->lastReceived() };
    QCOMPARE( spy.count(), 1 );
    QVERIFY( strcmp(data.toStdString().c_str(), lastReceived.c_str()) == 0 );
}

void TestNetwork::test_tcp_comm_server_to_socket()
{
    if( !ENABLE_TCP_TEST ) QSKIP("TCP test skipped");
    QSharedPointer<IfceIpSocket> remoteSocket{ createSocket(TCP_SOCKET) };
    QSignalSpy spy (remoteSocket.data(), SIGNAL(signalMessageReceived(QByteArray)));
    runEventLoop(1000);
    const QString data { "random send\n\n data \t\tHelloWorld" };
    server->send(data);
    runEventLoop(100);

    const std::string lastReceived { remoteSocket->lastServerResponse() };
    QCOMPARE( spy.count(), 1 );
    QVERIFY( strcmp(data.toStdString().c_str(), lastReceived.c_str()) == 0 );
}

void TestNetwork::test_tcp_comm_send_2_pkgs_at_once_sock_to_serv()
{
    if( !ENABLE_TCP_TEST ) QSKIP("TCP test skipped");
    QSharedPointer<IfceIpSocket> remoteSocket{ createSocket(TCP_SOCKET) };
    QSignalSpy spy(server.data(), SIGNAL(signalMessageReceived(QByteArray)));
    runEventLoop(1000);
    remoteSocket->send(QString("data1"));
    remoteSocket->send(QString("data2"));
    runEventLoop(20);
    QCOMPARE( spy.count(), 2 );
}

void TestNetwork::test_tcp_comm_send_2_pkgs_at_once_serv_to_sock()
{
    if( !ENABLE_TCP_TEST ) QSKIP("TCP test skipped");
    QSharedPointer<IfceIpSocket> remoteSocket{ createSocket(TCP_SOCKET) };
    QSignalSpy spy (remoteSocket.data(), SIGNAL(signalMessageReceived(QByteArray)));
    runEventLoop(1000);
    server->send(QString("data1"));
    server->send(QString("data2"));
    runEventLoop(20);
    QCOMPARE( spy.count(), 2 );
}

void TestNetwork::test_tcp_comm_socket_to_server_std_string()
{
    if( !ENABLE_TCP_TEST ) QSKIP("TCP test skipped");
    QSharedPointer<IfceIpSocket> remoteSocket{ createSocket(TCP_SOCKET) };
    runEventLoop(1000);
    const std::string data { "random send\n\n data \t\tHelloWorld" };
    remoteSocket->send(data);
    runEventLoop(20);

    const std::string lastReceived { server->lastReceived() };
    QVERIFY( strcmp(data.c_str(), lastReceived.c_str()) == 0 );
}

void TestNetwork::test_tcp_comm_server_to_socket_std_string()
{
    if( !ENABLE_TCP_TEST ) QSKIP("TCP test skipped");
    QSharedPointer<IfceIpSocket> remoteSocket{ createSocket(TCP_SOCKET) };
    runEventLoop(1000);
    const std::string data { "random send\n\n data \t\tHelloWorld" };
    server->send(data);
    runEventLoop(20);

    const std::string lastReceived { remoteSocket->lastServerResponse() };
    QVERIFY( strcmp(data.c_str(), lastReceived.c_str()) == 0 );
}

void TestNetwork::test_tcp_comm_socket_to_server_std_protobuf()
{
    if( !ENABLE_TCP_TEST ) QSKIP("TCP test skipped");
    QSharedPointer<IfceIpSocket> remoteSocket{ createSocket(TCP_SOCKET) };
    runEventLoop(1000);
    NetTest::TestMsg msg;
    msg.set_text("Hello world");
    msg.set_integervalue(128);
    msg.set_sample(NetTest::TestMsg::First);
    auto serialized = msg.SerializeAsString();
    remoteSocket->send(serialized);
    runEventLoop(20);

    const std::string lastReceived { server->lastReceived() };
    QVERIFY( strcmp(serialized.c_str(), lastReceived.c_str()) == 0 );
    NetTest::TestMsg received;
    received.ParseFromString(lastReceived);
    QVERIFY( google::protobuf::util::MessageDifferencer::Equals(msg, received) );
}

void TestNetwork::test_tcp_comm_server_to_socket_std_protobuf()
{
    if( !ENABLE_TCP_TEST ) QSKIP("TCP test skipped");
    QSharedPointer<IfceIpSocket> remoteSocket{ createSocket(TCP_SOCKET) };
    runEventLoop(1000);
    NetTest::TestMsg msg;
    msg.set_text("Hello world");
    msg.set_integervalue(95841);
    msg.set_sample(NetTest::TestMsg::First);
    auto serialized = msg.SerializeAsString();
    server->send(serialized);
    runEventLoop(20);

    const std::string lastReceived { remoteSocket->lastServerResponse() };
    QVERIFY( strcmp(serialized.c_str(), lastReceived.c_str()) == 0 );
    NetTest::TestMsg received;
    received.ParseFromString(lastReceived);
    QVERIFY( google::protobuf::util::MessageDifferencer::Equals(msg, received) );
}

void TestNetwork::test_tcp_comm_reconnection_to_server()
{
    if( !ENABLE_TCP_TEST ) QSKIP("TCP test skipped");
    constexpr int testServerPort = 3433;
    QSharedPointer<IfceIpSocket> remoteSocket = QSharedPointer<TcpSocket>::create(socketAddress, 0, this);
    remoteSocket->connectToHost(QHostAddress::LocalHost, testServerPort);
    runEventLoop(1000);
    auto serv = QSharedPointer<TcpServer>::create(QHostAddress::LocalHost, testServerPort);
    Q_UNUSED(serv);
    runEventLoop(1000);
    QVERIFY(remoteSocket->isOpen());
}

void TestNetwork::test_udp_comm_socket_to_server()
{
    if( !ENABLE_UDP_TEST ) QSKIP("UDP test skipped");
    const std::string testText { "hello \n\t\r world...<?_*>\n  t" };
    QSharedPointer<IfceIpSocket> udpClient = QSharedPointer<UdpSocket>::create(socketAddress, socketPort, this);
    QSignalSpy spy(udpServer.data(), SIGNAL(signalMessageReceived(QByteArray)));
    udpClient->connectToHost(serverAddress, serverPort);
    runEventLoop(5);
    udpClient->send(testText);
    runEventLoop(20);
    auto received { udpServer->lastReceived() };
    QCOMPARE( spy.count(), 1 );
    QCOMPARE( received, testText );
}

void TestNetwork::test_udp_comm_server_to_socket()
{
    if( !ENABLE_UDP_TEST ) QSKIP("UDP test skipped");
    QSharedPointer<IfceIpSocket> remoteSocket{ createSocket(UDP_SOCKET) };
    QSignalSpy spy (remoteSocket.data(), SIGNAL(signalMessageReceived(QByteArray)));
    runEventLoop(5);
    const QString data { "random send\n\n data \t\tHelloWorld" };
    udpServer->send(data);
    runEventLoop(20);

    const std::string lastReceived { remoteSocket->lastServerResponse() };
    QCOMPARE( spy.count(), 1 );
    QVERIFY( strcmp(data.toStdString().c_str(), lastReceived.c_str()) == 0 );
}

void TestNetwork::test_udp_comm_send_2_pkgs_at_once_sock_to_serv()
{
    if( !ENABLE_UDP_TEST ) QSKIP("UDP test skipped");
    QSharedPointer<IfceIpSocket> remoteSocket{ createSocket(UDP_SOCKET) };
    QSignalSpy spy(udpServer.data(), SIGNAL(signalMessageReceived(QByteArray)));
    runEventLoop(5);
    remoteSocket->send(QString("data1"));
    remoteSocket->send(QString("data2"));
    runEventLoop(20);
    QCOMPARE( spy.count(), 2 );
}

void TestNetwork::test_udp_comm_send_2_pkgs_at_once_serv_to_sock()
{
    if( !ENABLE_UDP_TEST ) QSKIP("UDP test skipped");
    QSharedPointer<IfceIpSocket> remoteSocket{ createSocket(UDP_SOCKET) };
    QSignalSpy spy (remoteSocket.data(), SIGNAL(signalMessageReceived(QByteArray)));
    runEventLoop(5);
    udpServer->send(QString("data1"));
    udpServer->send(QString("data2"));
    runEventLoop(20);
    QCOMPARE( spy.count(), 2 );
}

void TestNetwork::test_udp_comm_socket_to_server_std_protobuf()
{
    if( !ENABLE_UDP_TEST ) QSKIP("UDP test skipped");
    QSharedPointer<IfceIpSocket> remoteSocket{ createSocket(UDP_SOCKET) };
    runEventLoop(5);
    NetTest::TestMsg msg;
    msg.set_text("Hello world");
    msg.set_integervalue(128);
    msg.set_sample(NetTest::TestMsg::First);
    auto serialized = msg.SerializeAsString();
    remoteSocket->send(serialized);
    runEventLoop(20);

    const std::string lastReceived { udpServer->lastReceived() };
    QVERIFY( strcmp(serialized.c_str(), lastReceived.c_str()) == 0 );
    NetTest::TestMsg received;
    received.ParseFromString(lastReceived);
    QVERIFY( google::protobuf::util::MessageDifferencer::Equals(msg, received) );
}

void TestNetwork::test_udp_comm_server_to_socket_std_protobuf()
{
    if( !ENABLE_UDP_TEST ) QSKIP("UDP test skipped");
    QSharedPointer<IfceIpSocket> remoteSocket{ createSocket(UDP_SOCKET) };
    runEventLoop(5);
    NetTest::TestMsg msg;
    msg.set_text("Hello world");
    msg.set_integervalue(95841);
    msg.set_sample(NetTest::TestMsg::First);
    auto serialized = msg.SerializeAsString();
    udpServer->send(serialized);
    runEventLoop(20);

    const std::string lastReceived { remoteSocket->lastServerResponse() };
    QVERIFY( strcmp(serialized.c_str(), lastReceived.c_str()) == 0 );
    NetTest::TestMsg received;
    received.ParseFromString(lastReceived);
    QVERIFY( google::protobuf::util::MessageDifferencer::Equals(msg, received) );
}


QTEST_MAIN(TestNetwork)

#include "tst_testnetwork.moc"
