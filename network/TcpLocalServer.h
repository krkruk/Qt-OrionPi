#ifndef TCPLOCALSERVER_H
#define TCPLOCALSERVER_H
#include <QScopedPointerObjectDeleteLater>
#include <QList>
#include <QPointer>
#include <QTcpSocket>
#include <QHostAddress>


#include "IfceLocalServer.h"

class QTcpServer;


class TcpLocalServer : public IfceLocalServer
{
public:
    TcpLocalServer(int port = 0, QObject *parent = nullptr);
    ~TcpLocalServer();


    void setListenAddress(const QHostAddress &address) override;
    void setListenPort(int port);
    void setMaxConnections(int maxConn) override;
    bool start() override;

    QByteArray getLastLocalMessage() const override;
    QByteArray getLastRemoteMessage() const override;


    void setIpcServer(QSharedPointer<IfceIpcServer> ipcServer) override;


public slots:
    void onRemoteMessageReceived(const QByteArray &data) override;
    void onLocalMessageReceived(const QByteArray &data) override;


private slots:
    void onNewConnection();


private:
    QScopedPointer<QTcpServer, QScopedPointerObjectDeleteLater<QTcpServer>> server;
    QList<QPointer<QTcpSocket>> sockets;

    QHostAddress address = QHostAddress::Any;
    int port {0};
    QSharedPointer<IfceIpcServer> ipcServer;
    QByteArray lastRemoteMessage;
    QByteArray lastLocalMessage;
};

#endif // TCPLOCALSERVER_H
