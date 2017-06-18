#ifndef TCPLOCALSOCKET_H
#define TCPLOCALSOCKET_H
#include <QHostAddress>
#include <QScopedPointer>
#include <QScopedPointerDeleteLater>
#include <QAbstractSocket>

#include "IfceLocalSocket.h"


class QTcpSocket;


class TcpLocalSocket : public IfceLocalSocket
{
    Q_OBJECT
    static constexpr int RECONNECT_INTERVAL = 100;

public:
    explicit TcpLocalSocket(const QHostAddress &addr, int port, const QString &tag, QObject *parent = nullptr);
    ~TcpLocalSocket();

    QByteArray getLastRemoteMessage() const override;
    QByteArray getLastLocalMessage() const override;


public slots:
    void onRemoteMessageReceived(const QByteArray &data) override;
    void onLocalMessageReceived(const QByteArray &data) override;


private slots:
    void onSocketError(QAbstractSocket::SocketError errCode);


private:
    void connections();

    QScopedPointer<QTcpSocket, QScopedPointerDeleteLater> socket;
    const QHostAddress address;
    const int port {0};
    QByteArray lastRemoteMessage;
    QByteArray lastLocalMessage;
};

#endif // TCPLOCALSOCKET_H
