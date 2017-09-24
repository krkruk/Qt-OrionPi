#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QAbstractSocket>
#include <QDataStream>
#include "include/IfceServer.h"

class QTcpServer;
class QTcpSocket;
class QHostAddress;

class TcpServer : public IfceServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    TcpServer(const QHostAddress &address, int port, QObject *parent = nullptr);
    ~TcpServer();

    void listen(const QHostAddress &address, int port) override;
    void send(const QString &data) override;
    void send(const std::string &data) override;
    void send(const QByteArray &data) override;
    void setMaxPendingConnections(int numConnections) override;
    std::string lastReceived() const override;


private slots:
    void _on_new_connection_received();
    void _on_socket_data_received();
    void _on_socket_error(QAbstractSocket::SocketError socketError);

private:
    void connections();
    void connections_socket();
    void write_to_server(const QByteArray &data);

    QScopedPointer<QTcpServer> server;
    QSharedPointer<QTcpSocket> currentSocket;

    QDataStream in;
    std::string lastReceivedData;
};

#endif // TCPSERVER_H
