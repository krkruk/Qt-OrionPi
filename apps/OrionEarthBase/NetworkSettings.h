#ifndef NETWORKSETTINGS_H
#define NETWORKSETTINGS_H

#include <QDialog>

class QHostAddress;


namespace Ui {
class NetworkSettings;
}

class NetworkSettings : public QDialog
{
    Q_OBJECT

public:
    explicit NetworkSettings(const QString &organization,
                             const QString &application,
                             QWidget *parent = 0);
    ~NetworkSettings();

    QHostAddress getBindAddress() const;
    int getBindPort() const;

    QHostAddress getConnectToAddress() const;
    int getConnectToPort() const;


private:
    Ui::NetworkSettings *ui;
    QString organization;
    QString application;
};

#endif // NETWORKSETTINGS_H
