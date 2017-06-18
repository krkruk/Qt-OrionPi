#include "TcpGlobalServerFactory.h"
#include "TcpGlobalServer.h"


TcpGlobalServerFactory::TcpGlobalServerFactory()
    : TcpGlobalServerFactory(QHostAddress::LocalHost, 0, "")
{    
}

TcpGlobalServerFactory::TcpGlobalServerFactory(const QHostAddress &address, int port, const QString &serverName)
    : address{ address }, port { port }, serverName { serverName }
{
}

void TcpGlobalServerFactory::setAddress(const QHostAddress &address)
{
    this->address = address;
}

void TcpGlobalServerFactory::setPort(int port)
{
    this->port = port;
}

void TcpGlobalServerFactory::setServerName(const QString &serverName)
{
    this->serverName = serverName;
}

QSharedPointer<IfceServer> TcpGlobalServerFactory::create()
{
    return QSharedPointer<TcpGlobalServer>::create(address, port);
}
