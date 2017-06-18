#ifndef JSONSERIALPARSER_H
#define JSONSERIALPARSER_H
#include <QVariant>
#include <QScopedPointer>

#include "exceptions/ParsingException.h"
#include "IfceSerialParser.h"
#include "SerialSettings.h"


class JsonSerialParser : public IfceSerialParser
{
    static const QString _ID;

    class SerialJsonParserPrivate;
    QScopedPointer<SerialJsonParserPrivate> var;
public:
    explicit JsonSerialParser();
    explicit JsonSerialParser(int id);
    ~JsonSerialParser();
    enum VALUES {
        ID,
        JSON_OBJECT
    };

    bool parse(const QByteArray &data) throw(ParsingException) override;
    int getId() const override;
    QVariant getValue(int valueId) const override;
};

#endif // JSONSERIALPARSER_H
