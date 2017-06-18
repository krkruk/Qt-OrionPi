#ifndef IFCELOCALSOCKET_H
#define IFCELOCALSOCKET_H
#include <QObject>
#include "IfceSocket.h"

class IfceLocalSocket : public IfceSocket
{
    Q_OBJECT
public:
    explicit IfceLocalSocket(const QString &tag, QObject *parent = nullptr)
        : IfceSocket(parent),
          tag { tag }
    {}
    virtual ~IfceLocalSocket(){}

    QString getTag() const { return tag; }


signals:
    void signalLocalMessage(const QByteArray &data);


private:
    const QString tag;
};

#endif // IFCELOCALSOCKET_H
