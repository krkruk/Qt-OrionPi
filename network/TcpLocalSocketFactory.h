#ifndef TCPLOCALSOCKETFACTORY_H
#define TCPLOCALSOCKETFACTORY_H

#include "IfceLocalSocketFactory.h"

class TcpLocalSocketFactory : public IfceLocalSocketFactory
{
public:
    TcpLocalSocketFactory();
    ~TcpLocalSocketFactory();

    void setAddress(const QHostAddress &address) override;
    void setPort(int port) override;
    void setServerName(const QString &serverName) override;
    void setTag(const QString &tag) override;

    QSharedPointer<IfceSocket> create() override;

private:
    QHostAddress address;
    int port {0};
    QString serverName;
    QString tag;
};

#endif // TCPLOCALSOCKETFACTORY_H
