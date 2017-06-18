#include "MockRemoteSocket.h"
#include <QTimer>
#include <QDebug>

MockRemoteSocket::MockRemoteSocket(const QHostAddress &addr, int port, QObject *parent)
    : QObject(parent),
      address(addr),
      port(port),
      socket( new QTcpSocket(this) )
{
    connect(socket.data(), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect(socket.data(), &QTcpSocket::readyRead, this, [&]{
       const auto data = socket->readAll();
       onReceiveData(data);
    });

    socket->connectToHost(address, port);
}

MockRemoteSocket::~MockRemoteSocket()
{
}

QByteArray MockRemoteSocket::getReceivedData() const
{
    return lastReceived;
}

void MockRemoteSocket::onSocketError(QAbstractSocket::SocketError code)
{
    if(code == QAbstractSocket::ConnectionRefusedError)
        QTimer::singleShot(50, this, [&]{socket->connectToHost(address, port);});
    else {
        if( socket->isOpen() ) socket->close();
        socket.reset();
    }
}

void MockRemoteSocket::onSendDataToServer(const QByteArray &data)
{
    if( socket && socket->isOpen() )
        socket->write(data);
}

void MockRemoteSocket::onReceiveData(const QByteArray &data)
{
    lastReceived = data;
}
