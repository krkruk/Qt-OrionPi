#ifndef IPCSOCKET_H
#define IPCSOCKET_H
#include <QScopedPointer>
#include <QScopedPointerDeleteLater>
#include <QLocalSocket>
#include <QList>

#include "IfceIpcSocket.h"


class IpcSocket : public IfceIpcSocket
{
    Q_OBJECT

    static constexpr int TIMEOUT_INTERVAL = 100;
public:
    IpcSocket(const QString &serverName, QObject *parent = nullptr);
    ~IpcSocket();

    QByteArray getLastRemoteMessage() const override;
    QByteArray getLastLocalMessage() const override;

    void addObserver(QWeakPointer<Observer> observer) override;
    void delObserver(QWeakPointer<Observer> observer) override;
    void notifyObservers() override;


public slots:
    void onLocalMessageReceived(const QByteArray &data) override;


protected slots:
    void onRemoteMessageReceived(const QByteArray &data) override;


private slots:
    void onSocketError(QLocalSocket::LocalSocketError socketError);


private:
    void connections();

    QScopedPointer<QLocalSocket, QScopedPointerDeleteLater> socket;
    QList<QWeakPointer<Observer>> observers;
    QString serverName;
    QByteArray lastRemoteMsg;
    QByteArray lastLocalMsg;
};

#endif // IPCSOCKET_H
