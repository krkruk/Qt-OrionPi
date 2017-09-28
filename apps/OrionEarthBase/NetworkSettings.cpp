#include "NetworkSettings.h"
#include "ui_NetworkSettings.h"
#include <QHostAddress>
#include <QSettings>


namespace
{
    constexpr char CONN_GROUP[] = "CONNECTION_SETTINGS";
    constexpr char BIND_IP[] = "BIND_TO_IP";
    constexpr char BIND_PORT[] = "BIND_TO_PORT";
    constexpr char CONNECT_IP[] = "CONNECT_TO_IP";
    constexpr char CONNECT_PORT[] = "CONNECT_TO_PORT";
}

NetworkSettings::NetworkSettings(const QString &organization, const QString &application, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetworkSettings),
    organization(organization),
    application(application)
{
    ui->setupUi(this);

    QSettings settings(organization, application, this);
    settings.beginGroup(CONN_GROUP);
    const QString bindAddress = settings.value(
                BIND_IP, QHostAddress(QHostAddress::Any).toString()).toString();
    const int bindPort = settings.value(
                BIND_PORT, 0).toInt();
    const QString connToAddress = settings.value(
                CONNECT_IP, QHostAddress(QHostAddress::LocalHost).toString()).toString();
    const int connToPort = settings.value(
                CONNECT_PORT, 5000).toInt();
    settings.endGroup();

    ui->lineEditBindIpAddress->setText(bindAddress);
    ui->lineEditBindPort->setText(QString::number(bindPort));

    ui->lineEditConnectIpAddress->setText(connToAddress);
    ui->lineEditConnectPort->setText(QString::number(connToPort));
}

NetworkSettings::~NetworkSettings()
{
    QSettings settings(organization, application, this);
    settings.beginGroup(CONN_GROUP);
    settings.setValue(BIND_IP, getBindAddress().toString());
    settings.setValue(BIND_PORT, getBindPort());
    settings.setValue(CONNECT_IP, getConnectToAddress().toString());
    settings.setValue(CONNECT_PORT, getConnectToPort());
    settings.endGroup();
    delete ui;
}

QHostAddress NetworkSettings::getBindAddress() const
{
    return QHostAddress(ui->lineEditBindIpAddress->text());
}

int NetworkSettings::getBindPort() const
{
    return ui->lineEditBindPort->text().toInt();
}

QHostAddress NetworkSettings::getConnectToAddress() const
{
    return QHostAddress(ui->lineEditConnectIpAddress->text());
}

int NetworkSettings::getConnectToPort() const
{
    return ui->lineEditConnectPort->text().toInt();
}
