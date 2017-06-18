#ifndef MOCKOBSERVER_H
#define MOCKOBSERVER_H
#include <QObject>

#include "Observer.h"


class MockObserver : public Observer
{
public:
    ~MockObserver() {}

    QByteArray getLastRemoteMessage() const;
    void update(const QByteArray &data) override;


private:
    QByteArray lastRemoteMsg;
};

QByteArray MockObserver::getLastRemoteMessage() const
{
    return lastRemoteMsg;
}

void MockObserver::update(const QByteArray &data)
{
    lastRemoteMsg = data;
}

#endif // MOCKOBSERVER_H
