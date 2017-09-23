#ifndef IFCEWHEELMODEL_H
#define IFCEWHEELMODEL_H
#include "include/interface/IfceSerialModel.h"

namespace Orion {

    class IfceWheelModel : public IfceSerialModel
    {
    public:
        IfceWheelModel(int id)
            : IfceSerialModel(id)
        {}
        virtual ~IfceWheelModel() {}

        virtual void updateAngularVelocity(double angularVelocity) = 0;
        virtual double getCurrentAngularVelocity() const = 0;
        virtual double getExpectedAngularVelocity() const = 0;
    };
}
#endif // IFCEWHEELMODEL_H
