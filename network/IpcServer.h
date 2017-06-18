#ifndef IPCSERVER_H
#define IPCSERVER_H
#include <QScopedPointer>
#include <QScopedPointerObjectDeleteLater>
#include <QPointer>
#include <QLocalSocket>
#include <QList>

#include "IfceIpcServer.h"

class QLocalServer;


class IpcServer : public IfceIpcServer
{
    Q_OBJECT
public:
    explicit IpcServer(QObject *parent = nullptr);
    explicit IpcServer(const QString &serverName, QObject *parent = nullptr);
    ~IpcServer();

    void setMaxConnections(int maxConn) override;
    bool start() override;
    QByteArray getLastLocalMessage() const override;

    void setServerName(const QString &serverName) override;
    QString getServerName() const override;
    QByteArray getLastRemoteMessage() const override;

public slots:
    void onRemoteMessageReceived(const QByteArray &data) override;
    void onLocalMessageReceived(const QByteArray &data) override;


private slots:
    void onNewConnection();
    void onSocketError(QLocalSocket::LocalSocketError socketError);


private:
    void connections();


    QScopedPointer<QLocalServer, QScopedPointerObjectDeleteLater<QLocalServer>> server;
    QList<QPointer<QLocalSocket>> sockets;
    QString serverName;
    QByteArray lastRemoteMessage;
    QByteArray lastLocalMessage;
};

#endif // IPCSERVER_H
