#ifndef PARSINGEXCEPTION_H
#define PARSINGEXCEPTION_H
#include <exception>
#include <stdexcept>
#include <QString>

class ParsingException : public std::runtime_error
{
public:
    ParsingException(const QString &errorString)
        : runtime_error(QString("%1: %2")
                        .arg("ParsingException")
                        .arg(errorString)
                        .toStdString()
                        .c_str())
    {}
};

#endif // PARSINGEXCEPTION_H
