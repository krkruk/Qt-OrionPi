#include "UdpSocket.h"
#include <QDataStream>
#include <QUdpSocket>
#include <QDebug>


UdpSocket::UdpSocket(const QHostAddress &bindAddress, int bindPort, QObject *parent)
    : IfceIpSocket(parent),
      client(new QUdpSocket(this)),
      bindAddress( bindAddress ),
      bindPort( bindPort )
{
    client->bind(bindAddress, bindPort);
    connections();
}

UdpSocket::~UdpSocket()
{

}

void UdpSocket::connectToHost(const QHostAddress &address, int port)
{
    client->disconnectFromHost();
    client->bind(bindAddress, bindPort);
    client->connectToHost(address, port);
    write_to_server("");
}

void UdpSocket::disconnectFromHost()
{
}

void UdpSocket::send(const QString &data)
{
    write_to_server(data.toLocal8Bit());
}

void UdpSocket::send(const std::string &data)
{
    const QByteArray d(data.c_str(), data.size());
    write_to_server(d);
}

void UdpSocket::send(const QByteArray &data)
{
    write_to_server(data);
}

std::string UdpSocket::lastServerResponse() const
{
    return lastDatagram;
}

bool UdpSocket::isOpen() const
{
    return true;
}

void UdpSocket::_on_ready_read_received()
{
    in.startTransaction();
    QByteArray data;    in >> data;

    if( !in.commitTransaction() ) return;
    lastDatagram = data.toStdString();
    emit signalMessageReceived(data);

    if( client && client->bytesAvailable() > 0 )
        _on_ready_read_received();
}

void UdpSocket::connections()
{
    connect(client.data(), &QUdpSocket::readyRead, this, &UdpSocket::_on_ready_read_received);

    in.setDevice(client.data());
    in.setVersion(QDataStream::Qt_5_8);
}

void UdpSocket::write_to_server(const QByteArray &data)
{
    QByteArray pkg;
    QDataStream out(&pkg, QIODevice::WriteOnly);
    out.setDevice(client.data());
    out.setVersion(QDataStream::Qt_5_8);
    out << data;
    client->write(pkg);
}
