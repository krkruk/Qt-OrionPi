#ifndef IFCECHASSISFEEDBACKGENERATOR_H
#define IFCECHASSISFEEDBACKGENERATOR_H
#include <QWeakPointer>

namespace Orion {
    class IfceWheelModel;

    class IfceChassisFeedbackGenerator
    {
    public:
        virtual ~IfceChassisFeedbackGenerator() {}

        virtual void addInput(QWeakPointer<IfceWheelModel> model) = 0;

        virtual QByteArray generate() = 0;
        virtual void clear() = 0;
    };

}

#endif // IFCECHASSISFEEDBACKGENERATOR_H
