#ifndef TCPINPUTSOURCE_H
#define TCPINPUTSOURCE_H
#include <QScopedPointer>
#include <QHostAddress>
#include "IfceInputSource.h"

class IfceServer;
class QHostAddress;


class TcpInputSource : public IfceInputSource
{
public:
    explicit TcpInputSource(const QString &organization,
                            const QString &application, QObject *parent = nullptr);

    ~TcpInputSource();

    void send(const QByteArray &data) override;
    SOURCE_TYPE type() const override;

private:
    void load_settings();
    void setup_server();
    void connections();
    void store_settings();

    QString organization;
    QString application;

    QScopedPointer<IfceServer, QScopedPointerDeleteLater> server;
    QHostAddress listenAddress;
    int listenPort {5000};
};

#endif // TCPINPUTSOURCE_H
