#ifndef IFCEGLOBALSERVER_H
#define IFCEGLOBALSERVER_H
#include <QSharedPointer>
#include <QHostAddress>
#include <QObject>

#include "IfceServer.h"

class IfceDispatchEngine;
class IfceLocalSocket;

class IfceGlobalServer : public IfceServer
{
    Q_OBJECT
public:
    explicit IfceGlobalServer(QObject *parent = nullptr)
        : IfceServer(parent)
    {}
    ~IfceGlobalServer() {}
    virtual void setAddress(const QHostAddress &address) = 0;
    virtual void setPort(int port) = 0;
    virtual void setDispatcher(QSharedPointer<IfceDispatchEngine> dispatcher) = 0;
    virtual void addLocalObserver(QSharedPointer<IfceLocalSocket> observer) = 0;
};

#endif // IFCEGLOBALSERVER_H
