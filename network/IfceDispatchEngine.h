#ifndef IFCEDISPATCHENGINE_H
#define IFCEDISPATCHENGINE_H
#include <QByteArray>
#include <QStringList>
#include <QList>

class IfceDispatchEngine
{
public:
    virtual ~IfceDispatchEngine(){}

    virtual int addTag(const QString &tag) = 0;
    virtual QList<int> addTag(const QStringList &tags) = 0;
    virtual QStringList getTags() const = 0;

    virtual bool dispatch(const QByteArray &data) = 0;
    virtual QByteArray getDispatched(int id) = 0;
    virtual QByteArray getDispatched(const QString &tag) = 0;

    virtual void encode(const QString &tag, const QByteArray &data) = 0;
    virtual void encode(int id, const QByteArray &data) = 0;
    virtual QByteArray getEncoded() = 0;

    virtual void clear() = 0;
};

#endif // IFCEDISPATCHENGINE_H
