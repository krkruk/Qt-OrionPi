#ifndef IFCECHASSISMODEL_H
#define IFCECHASSISMODEL_H
#include <QSharedPointer>

namespace Orion {
    class IfceWheelModel;
    class IfceDriveMode;


    class IfceChassisModel
    {
    public:
        ~IfceChassisModel() {}
        virtual void setDriveAlgorithm(QSharedPointer<IfceDriveMode> algorithm) = 0;
        virtual void addWheel(QSharedPointer<IfceWheelModel> wheel) = 0;
        virtual void notifyAll() = 0;
        virtual void updateModel(const QByteArray &data) = 0;
    };
}
#endif // IFCECHASSISMODEL_H
