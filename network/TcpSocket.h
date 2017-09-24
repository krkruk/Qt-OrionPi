#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <QScopedPointer>
#include <QAbstractSocket>
#include <QHostAddress>
#include <QDataStream>

#include "include/IfceIpSocket.h"

class QTcpSocket;


class TcpSocket : public IfceIpSocket
{
    Q_OBJECT
    static const int RECONNECT_INTERVAL;
public:
    TcpSocket(const QHostAddress &address, int port, QObject *parent=nullptr);
    ~TcpSocket();

    void connectToHost(const QHostAddress &address, int port) override;
    void disconnectFromHost() override;
    void send(const QString &data) override;
    void send(const std::string &data) override;
    std::string lastServerResponse() const override;
    bool isOpen() const override;


private slots:
    void _on_socket_reconnect();
    void _on_socket_connected();
    void _on_socket_error(QAbstractSocket::SocketError socketError);
    void _on_socket_ready_read();


private:
    void connections();
    void write_to_server(const char *data);

    QScopedPointer<QTcpSocket> socket;
    QHostAddress address;
    int port {0};

    QDataStream in;
    std::string lastReceived;
};

#endif // TCPSOCKET_H
