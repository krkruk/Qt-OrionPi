#ifndef TCPGLOBALSERVER_H
#define TCPGLOBALSERVER_H
#include <QScopedPointer>
#include <QScopedPointerDeleteLater>
#include <QList>
#include <QPointer>
#include <QTcpSocket>
#include <QHash>

#include "IfceGlobalServer.h"

class QTcpServer;


class TcpGlobalServer : public IfceGlobalServer
{
    Q_OBJECT
public:
    TcpGlobalServer(QObject *parent = nullptr);
    TcpGlobalServer(int port, QObject *parent = nullptr);
    TcpGlobalServer(const QHostAddress &address, int port, QObject *parent = nullptr);
    ~TcpGlobalServer();

    void setMaxConnections(int maxConn) override;

    void setAddress(const QHostAddress &address) override;
    void setPort(int port) override;

    bool start() override;

    void setDispatcher(QSharedPointer<IfceDispatchEngine> dispatcher) override;
    void addLocalObserver(QSharedPointer<IfceLocalSocket> observer) override;
    QByteArray getLastRemoteMessage() const override;
    QByteArray getLastLocalMessage() const override;


public slots:
    void onRemoteMessageReceived(const QByteArray &data) override;


protected slots:
    void onLocalMessageReceived(const QByteArray &data) override;


private slots:
    void onNewConnection();


private:
    void encode_to_sent_to_remote_socket();
    void notify_connected_remote_sockets_data_changed();

    QScopedPointer<QTcpServer, QScopedPointerDeleteLater> server;
    QList<QPointer<QTcpSocket>> remoteSockets;
    QHostAddress address = QHostAddress::Any;
    int port {0};

    QSharedPointer<IfceDispatchEngine> dispatcher;
    using TagToSocket = QHash<QString, QSharedPointer<IfceLocalSocket>>;
    using TagToLastReceivedData = QHash<QString, QByteArray>;
    TagToSocket sockets;
    TagToLastReceivedData lastLocalMessages;
    QByteArray lastLocalMessageEncoded;
    QByteArray lastRemoteMessage;
};

#endif // TCPGLOBALSERVER_H
