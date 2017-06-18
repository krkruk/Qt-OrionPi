#ifndef MOCKSERIALMODEL_H
#define MOCKSERIALMODEL_H
#include <QByteArray>
#include <QJsonDocument>
#include <QList>
#include "include/IfceSerialObserver.h"
#include "include/IfceSerialModel.h"

class MockSerialModel : public IfceSerialModel
{
    int value;
    QList<IfceSerialObserver*> observers;
public:
    MockSerialModel(int id) : IfceSerialModel(id)
    {}

    void update(const QByteArray &line) override
    {
        QJsonParseError error;
        QJsonDocument doc { QJsonDocument::fromJson(line, &error) };
        if( error.error != QJsonParseError::NoError ) {
            return;
        }

        auto jsonObj { doc.object() };
        value = jsonObj["value"].toInt();
    }
    void setValue(int value) { this->value = value; }
    int getValue() const { return value; }
    void addObserver(IfceSerialObserver *observer) override
    {
        observers.append(observer);
    }

    bool delObserver(IfceSerialObserver *observer) override
    {
        return observers.removeOne(observer);
    }

    void notifyObservers() override
    {
        const auto id { getId() };
        const auto msg { QString("{\"ID\":%1, \"value\":%2}\r\n")
                    .arg(id).arg(value).toLatin1() };

        for( auto *observer : observers )
            observer->send(id, msg);
    }
};

#endif // MOCKSERIALMODEL_H
