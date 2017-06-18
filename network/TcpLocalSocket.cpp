#include "TcpLocalSocket.h"
#include <QTimer>
#include <QTcpSocket>
#include <QDebug>


TcpLocalSocket::TcpLocalSocket(const QHostAddress &addr, int port, const QString &tag, QObject *parent)
    : IfceLocalSocket(tag, parent),
      socket( new QTcpSocket(this) ),
      address{ addr },
      port{ port }
{
    connections();
    socket->connectToHost(address, port);
}

TcpLocalSocket::~TcpLocalSocket()
{
}


QByteArray TcpLocalSocket::getLastRemoteMessage() const
{
    return lastRemoteMessage;
}

QByteArray TcpLocalSocket::getLastLocalMessage() const
{
    return lastLocalMessage;
}

void TcpLocalSocket::onRemoteMessageReceived(const QByteArray &data)
{
    lastRemoteMessage = data;
    if( socket && socket->isOpen() )
        socket->write(data);
}

void TcpLocalSocket::onLocalMessageReceived(const QByteArray &data)
{
    lastLocalMessage = data;
    emit signalLocalMessage(lastLocalMessage);
}

void TcpLocalSocket::onSocketError(QAbstractSocket::SocketError errCode)
{
    Q_UNUSED(errCode)
    QTimer::singleShot(RECONNECT_INTERVAL, this, [&]{ socket->connectToHost(address, port); });
}

void TcpLocalSocket::connections()
{
    connect(socket.data(), static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
          this, &TcpLocalSocket::onSocketError);
    connect(socket.data(), &QTcpSocket::readyRead, this, [&]{
       const auto data = socket->readAll();
       onLocalMessageReceived(data);
    });
}
