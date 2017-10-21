#include "UdpServer.h"
#include <QUdpSocket>
#include <QDebug>
#include <QNetworkDatagram>
#include <string>

UdpServer::UdpServer(const QHostAddress &bindAddress, int bindPort, QObject *parent)
    : IfceServer(parent),
      server( new QUdpSocket(this) ),
      bindAddress( bindAddress ),
      bindPort( bindPort )
{
    server->bind(bindAddress, bindPort);
    server->connectToHost(QHostAddress::LocalHost, 0);
    connections();
}

UdpServer::~UdpServer()
{

}


void UdpServer::listen(const QHostAddress &address, int port)
{
    server->bind(address, port);
}

void UdpServer::send(const QString &data)
{
    write_to_server(data.toLocal8Bit());
}

void UdpServer::send(const std::string &data)
{
    const QByteArray d(data.c_str(), data.size());
    write_to_server(d);
}

void UdpServer::send(const QByteArray &data)
{
    write_to_server(data);
}

void UdpServer::setMaxPendingConnections(int numConnections)
{
    Q_UNUSED(numConnections)
}

std::string UdpServer::lastReceived() const
{
    return lastDatagram;
}

void UdpServer::_on_ready_read_received()
{
    in.startTransaction();
    QByteArray data;
    in >> data;
    if( !in.commitTransaction() ) return;
    lastDatagram = data.toStdString();
    if( data.isEmpty() ) setup_conn_with_remote_socket();
    else emit signalMessageReceived(data);

    if( server && server->bytesAvailable() > 0 )
        _on_ready_read_received();

}

void UdpServer::connections()
{
    connect(server.data(), &QUdpSocket::readyRead, this, &UdpServer::_on_ready_read_received);

    in.setDevice(server.data());
    in.setVersion(QDataStream::Qt_5_8);
}

void UdpServer::setup_conn_with_remote_socket()
{
    QNetworkDatagram nd = server->receiveDatagram();
    clientAddress = nd.senderAddress();
    clientPort = nd.senderPort();
    server->disconnectFromHost();
    server->bind(bindAddress, bindPort);
    server->connectToHost(clientAddress, clientPort);
}

void UdpServer::write_to_server(const QByteArray &data)
{
    QByteArray pkg;
    QDataStream out(&pkg, QIODevice::WriteOnly);
    out.setDevice(server.data());
    out.setVersion(QDataStream::Qt_5_8);
    out << data;
    server->write(pkg);
}
