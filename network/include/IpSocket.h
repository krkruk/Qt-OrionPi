#ifndef IPSOCKET_H
#define IPSOCKET_H
#include <QObject>

class QHostAddress;

namespace std {
    typedef basic_string<char> string;
}

class IpSocket : public QObject {
    Q_OBJECT
public:
    IpSocket(QObject *parent = nullptr)
        : QObject(parent) {}

    ~IpSocket() {}

    virtual void connectToHost(const QHostAddress &address, int port) = 0;
    virtual void disconnectFromHost() = 0;
    virtual void send(const QString &data) = 0;
    virtual void send(const std::string &data) = 0;

    virtual std::string lastServerResponse() const = 0;
    virtual bool isOpen() const = 0;
};

#endif // IPSOCKET_H
