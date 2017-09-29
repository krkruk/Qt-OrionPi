#ifndef IFCEINPUTSOURCE_H
#define IFCEINPUTSOURCE_H
#include <QObject>

enum class SOURCE_TYPE {
    UNDEFINED,
    TCP,
    UDP,
    GAMEPAD
};

class IfceInputSource : public QObject
{
    Q_OBJECT
public:
    explicit IfceInputSource(QObject *parent = nullptr)
        : QObject(parent) {}
    virtual ~IfceInputSource(){}

    virtual void send(const QByteArray &data) = 0;
    virtual SOURCE_TYPE type() const = 0;

signals:
    void signalMessageReceived(const QByteArray &data);
};

#endif // IFCEINPUTSOURCE_H
