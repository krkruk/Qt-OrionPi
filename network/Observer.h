#ifndef OBSERVER_H
#define OBSERVER_H
#include <QObject>

class QByteArray;


class Observer : public QObject
{
    Q_OBJECT
public:
    explicit Observer(QObject *parent = nullptr)
        : QObject(parent)
    {}
    virtual ~Observer() {}

    virtual void update(const QByteArray &data) = 0;


signals:
    void signalLocalResponse(const QByteArray &data);
};

#endif // OBSERVER_H
