#include "PeripheralNode.h"
#include "IfceIpcServer.h"
#include "IpcServer.h"
#include "IfceLocalServerFactory.h"
#include "IfceLocalServer.h"

PeripheralNode::PeripheralNode(const QHostAddress &nodeIpAddress, int listenPort, const QString &ipcServerName, QObject *parent)
    : IfceServer(parent),
      nodeAddress{nodeIpAddress},
      listenPort{listenPort},
      ipcServerName{ipcServerName},
      ipcServer{ QSharedPointer<IpcServer>::create(ipcServerName) }
{
}

void PeripheralNode::setLocalServerFactory(QSharedPointer<IfceLocalServerFactory> factory)
{
    this->remoteServerFactory = factory;
}


void PeripheralNode::setMaxConnections(int maxConn)
{
    if( localServer ) localServer->setMaxConnections(maxConn);
    ipcServer->setMaxConnections(maxConn);
}

bool PeripheralNode::start()
{
    if( !remoteServerFactory ) return false;

    localServer = qSharedPointerDynamicCast<IfceLocalServer>(remoteServerFactory->create());
    if( !localServer ) return false;

    localServer->setIpcServer(ipcServer);

    if( localServer->start() )
        return ipcServer->start();

    return false;
}

QByteArray PeripheralNode::getLastLocalMessage() const
{
    return ipcServer ? ipcServer->getLastLocalMessage() : QByteArray();
}

QByteArray PeripheralNode::getLastRemoteMessage() const
{
    return localServer ? localServer->getLastRemoteMessage() : QByteArray();
}

void PeripheralNode::onRemoteMessageReceived(const QByteArray &data)
{
    if( localServer )
        localServer->onRemoteMessageReceived(data);
}

void PeripheralNode::onLocalMessageReceived(const QByteArray &data)
{
    if( ipcServer )
        ipcServer->onLocalMessageReceived(data);
}
