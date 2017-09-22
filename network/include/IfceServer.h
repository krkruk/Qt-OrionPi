#ifndef IFCESERVER_H
#define IFCESERVER_H
#include <QObject>

namespace std {
    typedef basic_string<char> string;
}


class IfceServer : public QObject {
    Q_OBJECT

public:
    IfceServer(QObject *parent = nullptr)
        : QObject(parent) {}
    ~IfceServer(){}

    virtual void send(const QString &data) = 0;
    virtual void send(const std::string &data) = 0;
    virtual void setMaxPendingConnections(int numConnections) = 0;
    virtual std::string lastReceived() const = 0;

signals:
    void signalMessageReceived(const std::string &message);
};

#endif // IFCESERVER_H
