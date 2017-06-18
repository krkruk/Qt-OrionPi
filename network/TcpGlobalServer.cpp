#include "TcpGlobalServer.h"
#include "IfceLocalSocket.h"
#include "IfceDispatchEngine.h"
#include <QTcpServer>
#include <QDebug>

TcpGlobalServer::TcpGlobalServer(QObject *parent)
    : TcpGlobalServer(QHostAddress::Any, 0, parent)
{    
}

TcpGlobalServer::TcpGlobalServer(int port, QObject *parent) :
    TcpGlobalServer(QHostAddress::Any, port, parent)
{
}

TcpGlobalServer::TcpGlobalServer(const QHostAddress &address, int port, QObject *parent)
    : IfceGlobalServer(parent),
      server( new QTcpServer(this) ),
      address( address ),
      port( port )
{
    connect(server.data(), &QTcpServer::newConnection, this, &TcpGlobalServer::onNewConnection);
}

TcpGlobalServer::~TcpGlobalServer()
{
    for( auto socket : remoteSockets )
        if( socket && socket->isOpen() )
            socket->close();
}

void TcpGlobalServer::setMaxConnections(int maxConn)
{
    server->setMaxPendingConnections(maxConn);
}

void TcpGlobalServer::setAddress(const QHostAddress &address)
{
    this->address = address;
}

void TcpGlobalServer::setPort(int port)
{
    this->port = port;
}

bool TcpGlobalServer::start()
{
    return server->listen(address, port);
}

void TcpGlobalServer::setDispatcher(QSharedPointer<IfceDispatchEngine> dispatcher)
{
    this->dispatcher = dispatcher;
}

void TcpGlobalServer::addLocalObserver(QSharedPointer<IfceLocalSocket> observer)
{
    sockets.insert(observer->getTag(), observer);
    connect(observer.data(), &IfceLocalSocket::signalLocalMessage, this, &TcpGlobalServer::onLocalMessageReceived);
}

QByteArray TcpGlobalServer::getLastRemoteMessage() const
{
    return lastRemoteMessage;
}

QByteArray TcpGlobalServer::getLastLocalMessage() const
{
    return lastLocalMessageEncoded;
}

void TcpGlobalServer::onRemoteMessageReceived(const QByteArray &data)
{
    lastRemoteMessage = data;
    if( dispatcher.isNull() ) return;
    dispatcher->dispatch(data);

    for( const auto &tag : dispatcher->getTags() )
        if( sockets.contains(tag) )
            sockets[tag]->onRemoteMessageReceived(dispatcher->getDispatched(tag));
}

void TcpGlobalServer::onLocalMessageReceived(const QByteArray &data)
{
    IfceLocalSocket *socket = qobject_cast<IfceLocalSocket*>(sender());
    const auto tag { socket->getTag() };
    lastLocalMessages.insert(tag, data);

    encode_to_sent_to_remote_socket();
    notify_connected_remote_sockets_data_changed();
}

void TcpGlobalServer::onNewConnection()
{
    QPointer<QTcpSocket> socket = server->nextPendingConnection();
    connect(socket.data(), static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
          [&]{
        QTcpSocket *s = qobject_cast<QTcpSocket*>(sender());
        if( s ) s->deleteLater();
    });

    connect(socket.data(), &QTcpSocket::readyRead, this, [&]{
        QTcpSocket *s = qobject_cast<QTcpSocket*>(sender());
        if( !s ) return;
        const auto data { s->readAll() };
        onRemoteMessageReceived(data);
    });

    remoteSockets.append(socket);
}

void TcpGlobalServer::encode_to_sent_to_remote_socket()
{
    if( !dispatcher ) return;

    auto elem = lastLocalMessages.constBegin();
    while( elem != lastLocalMessages.constEnd() ){
        const auto tag { elem.key() };
        const auto data { elem.value() };
        dispatcher->encode(tag, data);
        ++elem;
    }
    lastLocalMessageEncoded = dispatcher->getEncoded();
}

void TcpGlobalServer::notify_connected_remote_sockets_data_changed()
{
    for( auto socket : remoteSockets )
        if( socket && socket->isOpen() )
            socket->write(lastLocalMessageEncoded);
}


