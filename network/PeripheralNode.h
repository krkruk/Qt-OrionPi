#ifndef PERIPHERALNODE_H
#define PERIPHERALNODE_H

#include <QObject>
#include <QSharedPointer>
#include "IfceServer.h"

class IfceLocalServerFactory;
class IfceLocalServer;
class IfceIpcServer;


class PeripheralNode : public IfceServer
{
    Q_OBJECT
public:
    explicit PeripheralNode(const QHostAddress &nodeIpAddress, int listenPort, const QString &ipcServerName, QObject *parent = nullptr);
    void setLocalServerFactory(QSharedPointer<IfceLocalServerFactory> factory);


public:
    void setMaxConnections(int maxConn) override;
    bool start() override;
    QByteArray getLastLocalMessage() const override;
    QByteArray getLastRemoteMessage() const override;


public slots:
    void onRemoteMessageReceived(const QByteArray &data) override;
    void onLocalMessageReceived(const QByteArray &data) override;


private:
    QHostAddress nodeAddress;
    int listenPort {0};
    QString ipcServerName;

    QSharedPointer<IfceLocalServerFactory> remoteServerFactory;
    QSharedPointer<IfceLocalServer> localServer;
    QSharedPointer<IfceIpcServer> ipcServer;

    bool isStarted { false };
};

#endif // PERIPHERALNODE_H
