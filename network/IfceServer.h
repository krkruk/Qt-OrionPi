#ifndef IFCESERVER_H
#define IFCESERVER_H
#include <QHostAddress>
#include <QObject>


class IfceServer : public QObject
{
    Q_OBJECT
public:
    IfceServer(QObject *parent = nullptr)
        : QObject(parent)
    {}
    virtual ~IfceServer(){}

    virtual void setMaxConnections(int maxConn) = 0;
    virtual bool start() = 0;

    virtual QByteArray getLastLocalMessage() const = 0;
    virtual QByteArray getLastRemoteMessage() const = 0;


public slots:
    virtual void onRemoteMessageReceived(const QByteArray &data) = 0;
    virtual void onLocalMessageReceived(const QByteArray &data) = 0;
};

#endif // IFCESERVER_H
