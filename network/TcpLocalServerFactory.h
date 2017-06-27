#ifndef TCPLOCALSERVERFACTORY_H
#define TCPLOCALSERVERFACTORY_H
#include "IfceLocalServerFactory.h"

class TcpLocalServerFactory : public IfceLocalServerFactory
{
public:
    TcpLocalServerFactory();
    ~TcpLocalServerFactory();

    void setAddress(const QHostAddress &address) override;
    void setPort(int port) override;
    void setServerName(const QString &serverName) override;
    QSharedPointer<IfceServer> create() override;

private:
    QHostAddress address;
    int port{0};
    QString serverName;
};

#endif // TCPLOCALSERVERFACTORY_H
