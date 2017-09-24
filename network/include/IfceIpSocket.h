#ifndef IFCEIPSOCKET_H
#define IFCEIPSOCKET_H
#include <QObject>

class QHostAddress;

namespace std {
    typedef basic_string<char> string;
}

class IfceIpSocket : public QObject {
    Q_OBJECT
public:
    IfceIpSocket(QObject *parent = nullptr)
        : QObject(parent) {}

    ~IfceIpSocket() {}

    virtual void connectToHost(const QHostAddress &address, int port) = 0;
    virtual void disconnectFromHost() = 0;
    virtual void send(const QString &data) = 0;
    virtual void send(const std::string &data) = 0;

    virtual std::string lastServerResponse() const = 0;
    virtual bool isOpen() const = 0;

signals:
    void signalMessageReceived(const QByteArray &message);
};

#endif // IFCEIPSOCKET_H
