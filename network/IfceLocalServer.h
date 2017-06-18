#ifndef IFCELOCALSERVER_H
#define IFCELOCALSERVER_H
#include <QSharedPointer>

#include "IfceServer.h"

class IfceIpcServer;


class IfceLocalServer : public IfceServer
{
    Q_OBJECT
public:
    IfceLocalServer(QObject *parent = nullptr)
        : IfceServer(parent)
    {}
    ~IfceLocalServer() {}

    virtual void setListenAddress(const QHostAddress &address) = 0;
    virtual void setListenPort(int port) = 0;
    virtual void setIpcServer(QSharedPointer<IfceIpcServer> ipcServer) = 0;
};

#endif // IFCELOCALSERVER_H
