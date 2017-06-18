#include "RootNode.h"
#include "IfceDispatchEngine.h"
#include "IfceGlobalServer.h"
#include "IfceLocalSocketFactory.h"
#include "IfceServerFactory.h"
#include "IfceLocalSocket.h"


RootNode::RootNode(const QHostAddress &address, int port, QObject *parent)
    : QObject(parent), globalAddress(address), globalPort { port }
{
}

RootNode::~RootNode()
{
}

void RootNode::setDispatcher(QSharedPointer<IfceDispatchEngine> dispatcher)
{
    this->dispatchEngine = dispatcher;
}

void RootNode::setSocketFactory(QSharedPointer<IfceLocalSocketFactory> socketFactory)
{
    this->socketFactory = socketFactory;
}

void RootNode::setGlobalServerFactory(QSharedPointer<IfceServerFactory> server)
{
    this->serverFactory = server;
}

void RootNode::addSocket(const QHostAddress &socketAddr, int socketPort, const QString &socketTag)
{
    if( !socketFactory )
        return;

    socketFactory->setAddress(socketAddr);
    socketFactory->setPort(socketPort);
    socketFactory->setTag(socketTag);
    auto socket = socketFactory->create();
    if( auto localSocket = qSharedPointerDynamicCast<IfceLocalSocket>(socket) )
        localSockets.insert(socketTag, localSocket);
}

bool RootNode::start()
{
    if( !(dispatchEngine&&socketFactory&&serverFactory) )
        return false;

    dispatchEngine->clear();
    dispatchEngine->addTag(localSockets.keys());
    serverFactory->setAddress(globalAddress);
    serverFactory->setPort(globalPort);

    if( !(globalServer = qSharedPointerDynamicCast<IfceGlobalServer>(serverFactory->create())) )
            return false;

    globalServer->setDispatcher(dispatchEngine);

    for( auto localObserver : localSockets.values() )
        globalServer->addLocalObserver(localObserver);

    return globalServer->start();
}

QWeakPointer<IfceLocalSocket> RootNode::getSocket(const QString &socketTag) const
{
    auto socket = localSockets.value(socketTag);
    return socket;
}
