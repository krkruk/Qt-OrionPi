#ifndef MOCKSERIALMODEL_H
#define MOCKSERIALMODEL_H
#include <QByteArray>
#include <QJsonDocument>
#include <QList>
#include <QDebug>
#include "include/interface/IfceSerialObserver.h"
#include "include/interface/IfceSerialModel.h"

class MockSerialModel : public IfceSerialModel
{
    int value;
    QByteArray rawData;
    QList<QWeakPointer<IfceSerialObserver>> observers;
public:
    MockSerialModel(int id) : IfceSerialModel(id)
    {}

    void updateModel(const QByteArray &line) override
    {
        rawData = line;

        QJsonParseError error;
        QJsonDocument doc { QJsonDocument::fromJson(line, &error) };
        if( error.error != QJsonParseError::NoError ) {
            return;
        }

        auto jsonObj { doc.object() };
        value = jsonObj["ROT"].toInt();
    }
    void setValue(int value) { this->value = value; }
    int getValue() const { return value; }
    QByteArray getRawData() const { return rawData; }
    void addObserver(QWeakPointer<IfceSerialObserver> observer) override
    {
        observers.append(observer);
    }

    bool delObserver(QWeakPointer<IfceSerialObserver> observer) override
    {
        return observers.removeOne(observer);
    }

    void notifyObservers() override
    {
        const auto id { getId() };
        const auto msg { QString("{\"ID\":%1,\"ROT\":%2}\r\n")
                    .arg(id).arg(value).toLatin1() };

        for( auto &observer : observers )
            if( auto effector = observer.toStrongRef() )
                effector->send(id, msg);
    }
};

#endif // MOCKSERIALMODEL_H
