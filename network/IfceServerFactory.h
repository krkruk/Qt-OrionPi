#ifndef IFCEGLOBALSERVERFACTORY_H
#define IFCEGLOBALSERVERFACTORY_H
#include <QHostAddress>
#include <QSharedPointer>

class IfceServer;


class IfceServerFactory
{
public:
    virtual ~IfceServerFactory() {}

    virtual void setAddress(const QHostAddress &address) = 0;
    virtual void setPort(int port) = 0;
    virtual void setServerName(const QString &serverName) = 0;

    virtual QSharedPointer<IfceServer> create() = 0;
};

#endif // IFCEGLOBALSERVERFACTORY_H
