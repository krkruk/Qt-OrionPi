#include "TcpLocalSocketFactory.h"
#include "TcpLocalSocket.h"


TcpLocalSocketFactory::TcpLocalSocketFactory()
{   
}

TcpLocalSocketFactory::~TcpLocalSocketFactory()
{
}


void TcpLocalSocketFactory::setAddress(const QHostAddress &address)
{
    this->address = address;
}

void TcpLocalSocketFactory::setPort(int port)
{
    this->port = port;
}

void TcpLocalSocketFactory::setServerName(const QString &serverName)
{
    this->serverName = serverName;
}

void TcpLocalSocketFactory::setTag(const QString &tag)
{
    this->tag = tag;
}

QSharedPointer<IfceSocket> TcpLocalSocketFactory::create()
{
    return QSharedPointer<TcpLocalSocket>::create(address, port, tag);
}
