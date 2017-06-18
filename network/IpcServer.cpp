#include "IpcServer.h"
#include <QLocalServer>
#include <QLocalSocket>
#include <QDebug>


IpcServer::IpcServer(QObject *parent)
    : IpcServer("server", parent)
{}

IpcServer::IpcServer(const QString &serverName, QObject *parent)
    : IfceIpcServer(parent),
      server( new QLocalServer ),
      serverName(serverName)
{
    QLocalServer::removeServer(serverName);
    server->setSocketOptions(QLocalServer::UserAccessOption);
    connections();
}

IpcServer::~IpcServer()
{
    for( auto socket : sockets )
        if( socket && socket->isOpen() )
            socket->close();

    if( !server.isNull() )
        server->close();
}

void IpcServer::setMaxConnections(int maxConn)
{
    if( server.isNull() )
        server->setMaxPendingConnections(maxConn);
}

bool IpcServer::start()
{
    if( server.isNull() || serverName.isEmpty() )
        return false;

    return server->listen(serverName);
}

QByteArray IpcServer::getLastLocalMessage() const
{
    return lastLocalMessage;
}

void IpcServer::setServerName(const QString &serverName)
{
    this->serverName = serverName;
}

QString IpcServer::getServerName() const
{
    return server ? server->fullServerName()
                  : serverName ;
}

QByteArray IpcServer::getLastRemoteMessage() const
{
    return lastRemoteMessage;
}

void IpcServer::onRemoteMessageReceived(const QByteArray &data)
{
    lastRemoteMessage = data;

    for( auto socket : sockets )
        if( socket && socket->isOpen() )
            socket->write(data);
}

void IpcServer::onLocalMessageReceived(const QByteArray &data)
{
    lastLocalMessage = data;
    emit signalIpcUpdated(data);
}

void IpcServer::onNewConnection()
{
    QPointer<QLocalSocket> socket = server->nextPendingConnection();
    sockets.append(socket);
    connect(socket.data(), static_cast<void(QLocalSocket::*)(QLocalSocket::LocalSocketError)>(&QLocalSocket::error),
            this, &IpcServer::onSocketError);
    connect(socket.data(), &QLocalSocket::readyRead, this, [&] {
        QLocalSocket *s = qobject_cast<QLocalSocket*>(sender());
        if( s ) onLocalMessageReceived(s->readAll());
    });
}

void IpcServer::onSocketError(QLocalSocket::LocalSocketError socketError)
{
    Q_UNUSED(socketError)
    QLocalSocket *s = qobject_cast<QLocalSocket*>(sender());
    if( s->isOpen() )
        s->close();
    s->deleteLater();
}

void IpcServer::connections()
{
    connect(server.data(), &QLocalServer::newConnection, this, &IpcServer::onNewConnection);
}
