#include "TcpInputSource.h"
#include "TcpServer.h"
#include <QHostAddress>
#include <QSettings>
#include <QFile>

namespace
{
    const QHostAddress LISTEN_ADDRESS { QHostAddress::Any };
    constexpr int LISTEN_PORT { 5000 };

    constexpr char GROUP_SERVER_SETTINGS[] = "SERVER_SETUP";
    constexpr char TAG_LISTEN_ADDRESS[] = "LISTEN_IP_ADDRESS";
    constexpr char TAG_LISTEN_PORT[] = "LISTEN_IP_PORT";
}


TcpInputSource::TcpInputSource(const QString &organization, const QString &application, QObject *parent)
    : IfceInputSource(parent),
      organization(organization),
      application(application),
      server(new TcpServer(this))
{    
    connections();
    load_settings();
    setup_server();
}

TcpInputSource::~TcpInputSource()
{
    store_settings();
}

void TcpInputSource::send(const QByteArray &data)
{
    server->send(data);
}

SOURCE_TYPE TcpInputSource::type() const
{
    return SOURCE_TYPE::TCP;
}

void TcpInputSource::load_settings()
{
    QSettings settings(organization, application);
    settings.beginGroup(GROUP_SERVER_SETTINGS);
    listenAddress = QHostAddress(settings.value(TAG_LISTEN_ADDRESS,
                                                LISTEN_ADDRESS.toString()).toString());
    listenPort = settings.value(TAG_LISTEN_PORT, LISTEN_PORT).toInt();
    settings.endGroup();

    store_settings();
}

void TcpInputSource::setup_server()
{
    server->listen(listenAddress, listenPort);
}

void TcpInputSource::connections()
{
    connect(server.data(), &IfceServer::signalMessageReceived, this, &TcpInputSource::signalMessageReceived);
}

void TcpInputSource::store_settings()
{
    QSettings settings(organization, application);
    settings.beginGroup(GROUP_SERVER_SETTINGS);
    settings.setValue(TAG_LISTEN_ADDRESS, listenAddress.toString());
    settings.setValue(TAG_LISTEN_PORT, listenPort);
    settings.endGroup();
}
