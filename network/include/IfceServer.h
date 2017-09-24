#ifndef IFCESERVER_H
#define IFCESERVER_H
#include <QObject>
#include <string>
namespace std {
    typedef basic_string<char> string;
}
class QHostAddress;

class IfceServer : public QObject {
    Q_OBJECT

public:
    IfceServer(QObject *parent = nullptr)
        : QObject(parent) {}
    ~IfceServer(){}

    virtual void listen(const QHostAddress &address, int port) = 0;
    virtual void send(const QString &data) = 0;
    virtual void send(const std::string &data) = 0;
    virtual void send(const QByteArray &data) = 0;
    virtual void setMaxPendingConnections(int numConnections) = 0;
    virtual std::string lastReceived() const = 0;

signals:
    void signalMessageReceived(const QByteArray &message);
};

#endif // IFCESERVER_H
