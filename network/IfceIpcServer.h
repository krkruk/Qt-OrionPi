#ifndef IFCEIPCSERVER_H
#define IFCEIPCSERVER_H

#include "IfceServer.h"

class IfceIpcServer : public IfceServer
{
    Q_OBJECT
public:
    IfceIpcServer(QObject *parent = nullptr)
        : IfceServer(parent)
    {}
    ~IfceIpcServer() {}

    virtual void setServerName(const QString &serverName) = 0;
    virtual QString getServerName() const = 0;


signals:
    void signalIpcUpdated(const QByteArray &data);
};

#endif // IFCEIPCSERVER_H
