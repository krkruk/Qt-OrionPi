#include "JsonSerialParser.h"

#include <QJsonDocument>
#include <QJsonObject>

const QString JsonSerialParser::_ID = SerialSettings::instance()->getId();

class JsonSerialParser::SerialJsonParserPrivate {
private:
    friend class JsonSerialParser;
    QJsonObject jsonObj;

    int id {-1};
};

JsonSerialParser::JsonSerialParser()
    : var{new SerialJsonParserPrivate}
{
}

JsonSerialParser::JsonSerialParser(int id)
    : var{new SerialJsonParserPrivate}
{
    var->id = id;
}

JsonSerialParser::~JsonSerialParser()
{
}

bool JsonSerialParser::parse(const QByteArray &data) throw(ParsingException)
{
    QJsonParseError error;
    QJsonDocument doc { QJsonDocument::fromJson(data, &error) };
    if( error.error != QJsonParseError::NoError ) {
        throw ParsingException(error.errorString());
    }

    var->jsonObj = doc.object();
    var->id = var->jsonObj[_ID].toInt();
    return true;
}

int JsonSerialParser::getId() const
{
    return var->id;
}

QVariant JsonSerialParser::getValue(int valueId) const
{
    switch (valueId) {
    case ID:
        return var->id;
    case JSON_OBJECT:
        return var->jsonObj;
    default:
        return 0;
    }
}
