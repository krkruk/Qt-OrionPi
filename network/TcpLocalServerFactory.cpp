#include "TcpLocalServerFactory.h"
#include "TcpLocalServer.h"


TcpLocalServerFactory::TcpLocalServerFactory()
{

}

TcpLocalServerFactory::~TcpLocalServerFactory()
{
}


void TcpLocalServerFactory::setAddress(const QHostAddress &address)
{
    this->address = address;
}

void TcpLocalServerFactory::setPort(int port)
{
    this->port = port;
}

void TcpLocalServerFactory::setServerName(const QString &serverName)
{
    this->serverName = serverName;
}

QSharedPointer<IfceServer> TcpLocalServerFactory::create()
{
    auto tcpLocalServer { QSharedPointer<TcpLocalServer>::create() };
    tcpLocalServer->setListenAddress(this->address);
    tcpLocalServer->setListenPort(this->port);
    return tcpLocalServer;
}
