#ifndef MOCKREMOTESOCKET_H
#define MOCKREMOTESOCKET_H
#include <QObject>
#include <QTcpSocket>
#include <QScopedPointer>
#include <QScopedPointerDeleteLater>
#include <QHostAddress>


class MockRemoteSocket : public QObject
{
    Q_OBJECT
public:
    explicit MockRemoteSocket(const QHostAddress &addr, int port, QObject *parent =nullptr);
    ~MockRemoteSocket();
    QByteArray getReceivedData() const;

public slots:
    void onSendDataToServer(const QByteArray &data);


private slots:
    void onSocketError(QAbstractSocket::SocketError code);
    void onReceiveData(const QByteArray &data);

private:
    QHostAddress address;
    int port;
    QScopedPointer<QTcpSocket, QScopedPointerDeleteLater> socket;
    QByteArray lastReceived;
};

#endif // MOCKREMOTESOCKET_H
