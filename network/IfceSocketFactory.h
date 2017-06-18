#ifndef IFCESOCKETFACTORY_H
#define IFCESOCKETFACTORY_H
#include <QSharedPointer>
#include <QHostAddress>

class IfceSocket;


class IfceSocketFactory
{
public:
    virtual ~IfceSocketFactory(){}

    virtual void setAddress(const QHostAddress &address) = 0;
    virtual void setPort(int port) = 0;
    virtual void setServerName(const QString &serverName) = 0;

    virtual QSharedPointer<IfceSocket> create() = 0;
};

#endif // IFCESOCKETFACTORY_H
