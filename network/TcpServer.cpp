#include "TcpServer.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <string>

TcpServer::TcpServer(QObject *parent)
    : IfceServer(parent),
      server( new QTcpServer(this) )
{
    connections();
    server->setMaxPendingConnections(1);
}

TcpServer::TcpServer(const QHostAddress &address, int port, QObject *parent)
    : TcpServer(parent)
{
    server->listen(address, port);
}

TcpServer::~TcpServer()
{    
}

void TcpServer::listen(const QHostAddress &address, int port)
{
    server->listen(address, port);
}

void TcpServer::send(const QString &data)
{
    if( !currentSocket || !currentSocket->isOpen() ) return;

    write_to_server(data.toLocal8Bit());
}

void TcpServer::send(const std::string &data)
{
    if( !currentSocket || !currentSocket->isOpen() ) return;

    const QByteArray d(data.c_str(), data.size());
    write_to_server(d);
}

void TcpServer::send(const QByteArray &data)
{
    if( !currentSocket || !currentSocket->isOpen() ) return;

    write_to_server(data);
}

void TcpServer::setMaxPendingConnections(int numConnections)
{
    if( server )
        server->setMaxPendingConnections(numConnections);
}

std::string TcpServer::lastReceived() const
{
    return lastReceivedData;
}

void TcpServer::_on_new_connection_received()
{
    currentSocket = QSharedPointer<QTcpSocket>(server->nextPendingConnection(), [](QObject *o){ o->deleteLater();});
    if( !currentSocket ) return;
    in.resetStatus();
    in.setVersion(QDataStream::Qt_5_8);
    in.setDevice(currentSocket.data());
    connections_socket();
}

void TcpServer::_on_socket_data_received()
{
    in.startTransaction();
    QByteArray data;
    in >> data;
    if( !in.commitTransaction() ) return;
    lastReceivedData = data.toStdString();
    emit signalMessageReceived(data);
    if( currentSocket && currentSocket ->bytesAvailable() )
        _on_socket_data_received();
}

void TcpServer::_on_socket_error(QAbstractSocket::SocketError socketError)
{
    if( socketError != QAbstractSocket::RemoteHostClosedError )
        qDebug() << "Server-socket error:" << socketError;

    currentSocket.clear();
}

void TcpServer::connections()
{
    connect(server.data(), &QTcpServer::newConnection, this, &TcpServer::_on_new_connection_received);
}

void TcpServer::connections_socket()
{
    connect(currentSocket.data(), &QTcpSocket::readyRead, this, &TcpServer::_on_socket_data_received);
    connect(currentSocket.data(), static_cast< void(QAbstractSocket::*)(QAbstractSocket::SocketError) >(&QAbstractSocket::error),
            this, &TcpServer::_on_socket_error);
}

void TcpServer::write_to_server(const QByteArray &data)
{
    if( currentSocket && !currentSocket->isOpen() )
        return;

    QByteArray pkg;
    QDataStream out(&pkg, QIODevice::ReadWrite);
    out.setDevice(currentSocket.data());
    out.setVersion(QDataStream::Qt_5_8);
    out << data;
    currentSocket->write(pkg);
}
