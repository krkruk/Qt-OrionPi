#ifndef IFCELOCALSOCKETFACTORY_H
#define IFCELOCALSOCKETFACTORY_H
#include "IfceSocketFactory.h"


class IfceLocalSocketFactory : public IfceSocketFactory
{
public:
    ~IfceLocalSocketFactory() {}

    virtual void setTag(const QString &tag) = 0;
};

#endif // IFCELOCALSOCKETFACTORY_H
