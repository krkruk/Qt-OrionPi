#ifndef JSONSERIALPARSER_H
#define JSONSERIALPARSER_H
#include <QVariant>
#include <QScopedPointer>

#include "exceptions/ParsingException.h"
#include "interface/IfceSerialParser.h"


class JsonSerialParser : public IfceSerialParser
{
    const QString _ID;

    class SerialJsonParserPrivate;
    QScopedPointer<SerialJsonParserPrivate> var;
public:
    explicit JsonSerialParser(const QString &idTag);
    explicit JsonSerialParser(const QString &idTag, int id);
    ~JsonSerialParser();
    enum VALUES {
        ID,
        JSON_OBJECT
    };

    bool parse(const QByteArray &data) override;
    int getId() const override;
    QVariant getValue(int valueId) const override;
};

#endif // JSONSERIALPARSER_H
