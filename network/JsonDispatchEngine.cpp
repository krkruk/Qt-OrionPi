#include "JsonDispatchEngine.h"
#include <QHash>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QVariant>
#include <QDebug>

#include <algorithm>

class JsonDispatchEngine::JsonDispatchEnginePrivate
{
    friend class JsonDispatchEngine;
public:
    JsonDispatchEnginePrivate();
    JsonDispatchEnginePrivate(const JsonDispatchEnginePrivate &other);
    JsonDispatchEnginePrivate(JsonDispatchEnginePrivate &&other) = default;
    JsonDispatchEnginePrivate& operator=(const JsonDispatchEnginePrivate &other);
    JsonDispatchEnginePrivate& operator=(JsonDispatchEnginePrivate &&) = default;
    ~JsonDispatchEnginePrivate();

    bool dispatch(const QByteArray &dispatchData) noexcept;
    QByteArray encode() noexcept;

private:
    using IdToData = QHash<int, QByteArray>;
    using TagToId = QHash<QString, int>;
    using TagToData = QHash<QString, QByteArray>;
    IdToData dispatchData;
    TagToData encodeData;
    TagToId tags;
};

JsonDispatchEngine::JsonDispatchEngine()
    : d_ptr{ new JsonDispatchEnginePrivate }
{    
}

JsonDispatchEngine::JsonDispatchEngine(const JsonDispatchEngine &other)
    : d_ptr{ new JsonDispatchEnginePrivate(*other.d_ptr) }
{
}

JsonDispatchEngine::JsonDispatchEngine(JsonDispatchEngine &&) = default;

JsonDispatchEngine& JsonDispatchEngine::operator=(const JsonDispatchEngine &other)
{
    if( this != &other )
        d_ptr.reset(new JsonDispatchEnginePrivate(*other.d_ptr));
    return *this;
}

JsonDispatchEngine& JsonDispatchEngine::operator=(JsonDispatchEngine &&) = default;

JsonDispatchEngine::~JsonDispatchEngine()
{
}

int JsonDispatchEngine::addTag(const QString &tag)
{
    const int id { d_ptr->dispatchData.count() + 1 };
    d_ptr->dispatchData.insert(id, QByteArray());
    d_ptr->tags.insert(tag, id);
    return id;
}

QList<int> JsonDispatchEngine::addTag(const QStringList &tags)
{
    QList<int> result;
    std::for_each(tags.constBegin(), tags.constEnd(),
                  [&](const QString &t) { result.append(addTag(t)); });

    return result;
}

QStringList JsonDispatchEngine::getTags() const
{
    return d_ptr->tags.keys();
}

bool JsonDispatchEngine::dispatch(const QByteArray &data)
{
    return d_ptr->dispatch(data);
}

QByteArray JsonDispatchEngine::getDispatched(int id)
{
    return d_ptr->dispatchData.value(id);
}

QByteArray JsonDispatchEngine::getDispatched(const QString &tag)
{
    const auto id { d_ptr->tags.value(tag) };
    return d_ptr->dispatchData.value(id);
}

void JsonDispatchEngine::encode(const QString &tag, const QByteArray &data)
{
    d_ptr->encodeData.insert(tag, data);
}

void JsonDispatchEngine::encode(int id, const QByteArray &data)
{
    const QString tag = d_ptr->tags.key(id);
    if( !tag.isEmpty() )
        d_ptr->encodeData.insert(tag, data);
}

QByteArray JsonDispatchEngine::getEncoded()
{
    return d_ptr->encode();
}

void JsonDispatchEngine::clear()
{
    d_ptr->dispatchData.clear();
    d_ptr->encodeData.clear();
    d_ptr->tags.clear();
}

JsonDispatchEngine::JsonDispatchEnginePrivate::JsonDispatchEnginePrivate() = default;

JsonDispatchEngine::JsonDispatchEnginePrivate::JsonDispatchEnginePrivate(const JsonDispatchEngine::JsonDispatchEnginePrivate &other)
    : dispatchData{ other.dispatchData }, tags{ other.tags }
{
}

JsonDispatchEngine::JsonDispatchEnginePrivate& JsonDispatchEngine::JsonDispatchEnginePrivate::operator=(const JsonDispatchEngine::JsonDispatchEnginePrivate &other)
{
    if( this == &other )
        return *this;

    dispatchData = other.dispatchData;
    tags = other.tags;
    return *this;
}

JsonDispatchEngine::JsonDispatchEnginePrivate::~JsonDispatchEnginePrivate()
{

}

bool JsonDispatchEngine::JsonDispatchEnginePrivate::dispatch(const QByteArray &data) noexcept
{
    QJsonParseError err;
    QJsonDocument doc { QJsonDocument::fromJson(data, &err) };
    if( err.error != QJsonParseError::NoError ) return false;
    QJsonObject obj { doc.object() };

    auto elem { tags.constBegin() };
    while (elem != tags.constEnd()) {
        const auto tag { elem.key() };
        const auto id { elem.value() };
        const auto dispatched { QJsonDocument(obj.value(tag).toObject()) };
        this->dispatchData.insert(id, dispatched.toJson(QJsonDocument::Compact));
        ++elem;
    }

    return true;
}

QByteArray JsonDispatchEngine::JsonDispatchEnginePrivate::encode() noexcept
{
    if( encodeData.isEmpty() )
        return QByteArray();

    QJsonObject obj;
    auto elem { encodeData.constBegin() };
    while (elem != encodeData.constEnd()) {
        const auto tag { elem.key() };
        const auto data { elem.value() };
        auto d = QJsonDocument::fromJson(data);
        obj.insert(tag, d.object());
        ++elem;
    }

    QJsonDocument doc(obj);
    encodeData.clear();
    return doc.toJson(QJsonDocument::Compact);
}
