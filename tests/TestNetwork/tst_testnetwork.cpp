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

#include "include/IfceServer.h"
#include "include/IpSocket.h"
#include "TcpServer.h"
#include "TcpSocket.h"

#include <google/protobuf/util/json_util.h>
#include <google/protobuf/util/message_differencer.h>
#include "proto/testMsg.pb.h"

class TestNetwork : public QObject
{
    Q_OBJECT
    const QHostAddress serverAddress { QHostAddress::LocalHost };
    const QHostAddress socketAddress { QHostAddress::LocalHost };
    const int serverPort {5500};
    const int socketPort {5510};

    QSharedPointer<IfceServer> server;
public:
    TestNetwork();

    void runEventLoop(int millis) {
        QEventLoop loop;
        QTimer::singleShot(millis, &loop, SLOT(quit()));
        loop.exec();
    }

    QSharedPointer<IpSocket> createSocket() {
        QSharedPointer<IpSocket> remoteSocket{ QSharedPointer<TcpSocket>::create(socketAddress, 0, this) };
        remoteSocket->connectToHost(serverAddress, serverPort);
        return remoteSocket;
    }

private Q_SLOTS:
    void initTestCase();
    void test_tcp_comm_socket_to_server();
    void test_tcp_comm_server_to_socket();
    void test_tcp_comm_socket_to_server_std_string();
    void test_tcp_comm_server_to_socket_std_string();
    void test_tcp_comm_socket_to_server_std_protobuf();
    void test_tcp_comm_server_to_socket_std_protobuf();
    void test_tcp_comm_reconnection_to_server();
};

TestNetwork::TestNetwork()
    : server{ QSharedPointer<TcpServer>::create(serverAddress, serverPort, this) }
{
}

void TestNetwork::initTestCase()
{
}

void TestNetwork::test_tcp_comm_socket_to_server()
{
    QSharedPointer<IpSocket> remoteSocket{ createSocket() };
    runEventLoop(55);
    const QString data { "random send\n\n data \t\tHelloWorld" };
    remoteSocket->send(data);
    runEventLoop(20);

    const std::string lastReceived { server->lastReceived() };
    QVERIFY( strcmp(data.toStdString().c_str(), lastReceived.c_str()) == 0 );
}

void TestNetwork::test_tcp_comm_server_to_socket()
{
    QSharedPointer<IpSocket> remoteSocket{ createSocket() };
    runEventLoop(55);
    const QString data { "random send\n\n data \t\tHelloWorld" };
    server->send(data);
    runEventLoop(20);

    const std::string lastReceived { remoteSocket->lastServerResponse() };
    QVERIFY( strcmp(data.toStdString().c_str(), lastReceived.c_str()) == 0 );
}

void TestNetwork::test_tcp_comm_socket_to_server_std_string()
{
    QSharedPointer<IpSocket> remoteSocket{ createSocket() };
    runEventLoop(55);
    const std::string data { "random send\n\n data \t\tHelloWorld" };
    remoteSocket->send(data);
    runEventLoop(20);

    const std::string lastReceived { server->lastReceived() };
    QVERIFY( strcmp(data.c_str(), lastReceived.c_str()) == 0 );
}

void TestNetwork::test_tcp_comm_server_to_socket_std_string()
{
    QSharedPointer<IpSocket> remoteSocket{ createSocket() };
    runEventLoop(55);
    const std::string data { "random send\n\n data \t\tHelloWorld" };
    server->send(data);
    runEventLoop(20);

    const std::string lastReceived { remoteSocket->lastServerResponse() };
    QVERIFY( strcmp(data.c_str(), lastReceived.c_str()) == 0 );
}

void TestNetwork::test_tcp_comm_socket_to_server_std_protobuf()
{
    QSharedPointer<IpSocket> remoteSocket{ createSocket() };
    runEventLoop(55);
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
    QSharedPointer<IpSocket> remoteSocket{ createSocket() };
    runEventLoop(55);
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
    constexpr int testServerPort = 3433;
    QSharedPointer<IpSocket> remoteSocket = QSharedPointer<TcpSocket>::create(socketAddress, 0, this);
    remoteSocket->connectToHost(QHostAddress::LocalHost, testServerPort);
    runEventLoop(100);
    auto serv = QSharedPointer<TcpServer>::create(QHostAddress::LocalHost, testServerPort);
    Q_UNUSED(serv);
    runEventLoop(55);
    QVERIFY(remoteSocket->isOpen());
}


QTEST_MAIN(TestNetwork)

#include "tst_testnetwork.moc"
