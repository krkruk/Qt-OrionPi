#include "IpcSocket.h"
#include <QDebug>
#include <QTimer>

#include "Observer.h"


IpcSocket::IpcSocket(const QString &serverName, QObject *parent)
    : IfceIpcSocket(parent),
      socket( new QLocalSocket(this) ),
      serverName(serverName)
{
    connections();
    socket->connectToServer(serverName);
}

IpcSocket::~IpcSocket()
{

}

QByteArray IpcSocket::getLastRemoteMessage() const
{
    return lastRemoteMsg;
}

QByteArray IpcSocket::getLastLocalMessage() const
{
    return lastLocalMsg;
}

void IpcSocket::addObserver(QWeakPointer<Observer> observer)
{
    observers.append(observer);
    connect(observer.data(), &Observer::signalLocalResponse, this, &IpcSocket::onLocalMessageReceived);
}

void IpcSocket::delObserver(QWeakPointer<Observer> observer)
{
    observers.removeOne(observer);
}

void IpcSocket::notifyObservers()
{
    for( auto observer : observers ) {
        auto strongObserver { observer.toStrongRef() };
        strongObserver->update(lastRemoteMsg);
    }
}

void IpcSocket::onLocalMessageReceived(const QByteArray &data)
{
    lastLocalMsg = data;
    if( socket && socket->isOpen() )
        socket->write(lastLocalMsg);
}

void IpcSocket::onRemoteMessageReceived(const QByteArray &data)
{
    lastRemoteMsg = data;
    notifyObservers();
}

void IpcSocket::onSocketError(QLocalSocket::LocalSocketError socketError)
{
    switch( socketError ){
    case QLocalSocket::ConnectionRefusedError:
        QTimer::singleShot(TIMEOUT_INTERVAL, this, [&]{ socket->connectToServer(); });
        break;
    case QLocalSocket::ServerNotFoundError:
        QTimer::singleShot(TIMEOUT_INTERVAL, this, [&]{
            socket.reset(new QLocalSocket(this));
            connections();
            socket->connectToServer(serverName);
        });
    default:
        socket.reset();
    }
}

void IpcSocket::connections()
{
    connect(socket.data(), static_cast<void(QLocalSocket::*)(QLocalSocket::LocalSocketError)>(&QLocalSocket::error),
            this, &IpcSocket::onSocketError);
    connect(socket.data(), &QLocalSocket::readyRead, this, [&] {
        QLocalSocket *s = qobject_cast<QLocalSocket*>(sender());
        const auto data = s->readAll();
        onRemoteMessageReceived(data);
    });
    connect(socket.data(), &QLocalSocket::disconnected, socket.data(), &QLocalSocket::deleteLater);
}
