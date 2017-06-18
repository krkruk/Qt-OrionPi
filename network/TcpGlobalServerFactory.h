#ifndef TCPGLOBALSERVERFACTORY_H
#define TCPGLOBALSERVERFACTORY_H
#include "IfceServerFactory.h"


class TcpGlobalServerFactory : public IfceServerFactory
{
public:
    explicit TcpGlobalServerFactory();
    explicit TcpGlobalServerFactory(const QHostAddress &address, int port, const QString &serverName);

    void setAddress(const QHostAddress &address) override;
    void setPort(int port) override;
    void setServerName(const QString &serverName) override;

    QSharedPointer<IfceServer> create() override;

private:
    QHostAddress address;
    int port {0};
    QString serverName;
};

#endif // TCPGLOBALSERVERFACTORY_H
