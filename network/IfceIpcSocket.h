#ifndef IFCEIPCSOCKET_H
#define IFCEIPCSOCKET_H
#include <QObject>
#include "IfceSocket.h"
#include "Observable.h"


class IfceIpcSocket : public IfceSocket, public Observable
{
    Q_OBJECT
public:
    IfceIpcSocket(QObject *parent = nullptr)
        : IfceSocket(parent)
    {}
    ~IfceIpcSocket(){}

    virtual QByteArray getLastRemoteMessage() const = 0;
    virtual QByteArray getLastLocalMessage() const = 0;

public slots:
    virtual void onLocalMessageReceived(const QByteArray &data) = 0;


protected slots:
    virtual void onRemoteMessageReceived(const QByteArray &data) = 0;
};

#endif // IFCEIPCSOCKET_H
