#include "TcpLocalServer.h"
#include "IfceIpcServer.h"
#include <QTcpServer>
#include <QDebug>


TcpLocalServer::TcpLocalServer(int port, QObject *parent)
    : IfceLocalServer(parent),
      server( new QTcpServer(this) ),
      port { port }
{
    connect(server.data(), &QTcpServer::newConnection, this, &TcpLocalServer::onNewConnection);
}

TcpLocalServer::~TcpLocalServer()
{
}

void TcpLocalServer::setListenAddress(const QHostAddress &address)
{
    this->address = address;
}

void TcpLocalServer::setListenPort(int port)
{
    this->port = port;
}

void TcpLocalServer::setMaxConnections(int maxConn)
{
    if( server )
        server->setMaxPendingConnections(maxConn);
}

bool TcpLocalServer::start()
{
    return server->listen(address, port);
}

QByteArray TcpLocalServer::getLastLocalMessage() const
{
    return lastLocalMessage;
}

QByteArray TcpLocalServer::getLastRemoteMessage() const
{
    return lastRemoteMessage;
}

void TcpLocalServer::setIpcServer(QSharedPointer<IfceIpcServer> ipcServer)
{
    this->ipcServer = ipcServer;
    connect(this->ipcServer.data(), &IfceIpcServer::signalIpcUpdated,
            this, &TcpLocalServer::onLocalMessageReceived);
}


void TcpLocalServer::onRemoteMessageReceived(const QByteArray &data)
{
    lastRemoteMessage = data;
    if( !ipcServer )
        return;

    ipcServer->onRemoteMessageReceived(data);
}

void TcpLocalServer::onLocalMessageReceived(const QByteArray &data)
{
    lastLocalMessage = data;
    for( auto socket : sockets )
        if( socket && socket->isOpen() )
            socket->write(lastLocalMessage);
}

void TcpLocalServer::onNewConnection()
{
    QPointer<QTcpSocket> socket = server->nextPendingConnection();
    connect(socket.data(), static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
            this, [&](QAbstractSocket::SocketError){
        QTcpSocket *s = qobject_cast<QTcpSocket*>(sender());
        if( s ) return;
        if( s->isOpen() ) s->close();
        s->deleteLater();
    });
    connect(socket.data(), &QTcpSocket::readyRead, this, [&]{
        QTcpSocket *s = qobject_cast<QTcpSocket*>(sender());
        onRemoteMessageReceived(s->readAll());
    });
    sockets.append(socket);
}
