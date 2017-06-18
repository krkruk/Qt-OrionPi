#ifndef ROOTNODE_H
#define ROOTNODE_H
#include <QObject>
#include <QWeakPointer>
#include <QSharedPointer>
#include <QHostAddress>
#include <QHash>

#include "network_global.h"

class IfceDispatchEngine;
class IfceLocalSocketFactory;
class IfceServerFactory;

class IfceLocalSocket;
class IfceGlobalServer;

class NETWORKSHARED_EXPORT RootNode : public QObject
{
    Q_OBJECT
public:
    explicit RootNode(const QHostAddress &address, int port, QObject *parent=nullptr);
    ~RootNode();

    void setDispatcher(QSharedPointer<IfceDispatchEngine> dispatcher);
    void setSocketFactory(QSharedPointer<IfceLocalSocketFactory> socketFactory);
    void setGlobalServerFactory(QSharedPointer<IfceServerFactory> server);
    void addSocket(const QHostAddress &socketAddr, int socketPort, const QString &socketTag);
    bool start();

    QWeakPointer<IfceLocalSocket> getSocket(const QString &socketTag) const;

private:
    QHostAddress globalAddress;
    int globalPort {0};
    QSharedPointer<IfceDispatchEngine> dispatchEngine;
    QSharedPointer<IfceLocalSocketFactory> socketFactory;
    QSharedPointer<IfceServerFactory> serverFactory;

    using TagToSocket = QHash<QString, QSharedPointer<IfceLocalSocket>>;
    QSharedPointer<IfceGlobalServer> globalServer;
    TagToSocket localSockets;
};

#endif // ROOTNODE_H
