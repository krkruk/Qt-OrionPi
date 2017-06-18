#ifndef IFCESOCKET_H
#define IFCESOCKET_H
#include <QObject>


class IfceSocket : public QObject
{
    Q_OBJECT
public:
    explicit IfceSocket(QObject *parent = nullptr)
        : QObject(parent)
    {}
    virtual ~IfceSocket() {}

    virtual QByteArray getLastRemoteMessage() const = 0;
    virtual QByteArray getLastLocalMessage() const = 0;


public slots:
    virtual void onRemoteMessageReceived(const QByteArray &data) = 0;
    virtual void onLocalMessageReceived(const QByteArray &data) = 0;
};

#endif // IFCESOCKET_H
