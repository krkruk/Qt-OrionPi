#ifndef JSONDISPATCHENGINE_H
#define JSONDISPATCHENGINE_H
#include <memory>

#include "IfceDispatchEngine.h"


class JsonDispatchEngine : public IfceDispatchEngine
{
public:
    JsonDispatchEngine();
    JsonDispatchEngine(const JsonDispatchEngine &other);
    JsonDispatchEngine(JsonDispatchEngine &&);
    JsonDispatchEngine& operator=(const JsonDispatchEngine &other);
    JsonDispatchEngine& operator=(JsonDispatchEngine &&);
    ~JsonDispatchEngine();

    int addTag(const QString &tag) override;
    QList<int> addTag(const QStringList &tags) override;
    QStringList getTags() const override;

    bool dispatch(const QByteArray &data) override;
    QByteArray getDispatched(int id) override;
    QByteArray getDispatched(const QString &tag) override;

    void encode(const QString &tag, const QByteArray &data) override;
    void encode(int id, const QByteArray &data) override;
    QByteArray getEncoded() override;

    void clear();

private:
    class JsonDispatchEnginePrivate;
    std::unique_ptr<JsonDispatchEnginePrivate> d_ptr;
};

#endif // JSONDISPATCHENGINE_H
