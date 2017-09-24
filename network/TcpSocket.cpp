#include "TcpSocket.h"
#include <QDataStream>
#include <QTcpSocket>
#include <QTimer>
#include <QDebug>

const int TcpSocket::RECONNECT_INTERVAL = 25;


TcpSocket::TcpSocket(const QHostAddress &address, int port, QObject *parent)
    : IfceIpSocket(parent),
      socket( new QTcpSocket(this) )
{
    socket->bind(address, port);
    connections();
}

TcpSocket::~TcpSocket()
{

}

void TcpSocket::connectToHost(const QHostAddress &address, int port)
{
    this->address = address;
    this->port = port;
    _on_socket_reconnect();
}

void TcpSocket::disconnectFromHost()
{
    if( socket ) socket->disconnectFromHost();
}

void TcpSocket::send(const QString &data)
{
    write_to_server(data.toLocal8Bit());
}

void TcpSocket::send(const std::string &data)
{
    const QByteArray d(data.c_str(), data.size());
    write_to_server(d);
}

void TcpSocket::send(const QByteArray &data)
{
    write_to_server(data);
}

std::string TcpSocket::lastServerResponse() const
{
    return lastReceived;
}

bool TcpSocket::isOpen() const
{
    return socket->isOpen();
}

void TcpSocket::_on_socket_reconnect()
{
    socket->connectToHost(address, port);
}

void TcpSocket::_on_socket_connected()
{
    in.setDevice(socket.data());
    in.setVersion(QDataStream::Qt_5_8);
}

void TcpSocket::_on_socket_error(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Socket error:" << socketError << socket->errorString();
    QTimer::singleShot(RECONNECT_INTERVAL, this, &TcpSocket::_on_socket_reconnect);
}

void TcpSocket::_on_socket_ready_read()
{
    in.startTransaction();
    QByteArray data;
    in >> data;
    if( !in.commitTransaction() ) return;
    lastReceived = data.toStdString();
    emit signalMessageReceived(data);

    if( socket && socket->bytesAvailable() > 0 )
        _on_socket_ready_read();
}

void TcpSocket::connections()
{
    connect(socket.data(), &QTcpSocket::connected, this, &TcpSocket::_on_socket_connected);
    connect(socket.data(), static_cast< void(QAbstractSocket::*)(QAbstractSocket::SocketError) >(&QAbstractSocket::error),
            this, &TcpSocket::_on_socket_error);
    connect(socket.data(), &QTcpSocket::readyRead, this, &TcpSocket::_on_socket_ready_read);
}

void TcpSocket::write_to_server(const QByteArray &data)
{
    if( !isOpen() ) return;

    QByteArray pkg;
    QDataStream out(&pkg, QIODevice::WriteOnly);
    out.setDevice(socket.data());
    out.setVersion(QDataStream::Qt_5_8);
    out << data;
    socket->write(pkg);
}
