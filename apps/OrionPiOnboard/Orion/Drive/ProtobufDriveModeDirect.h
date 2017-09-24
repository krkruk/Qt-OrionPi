#ifndef PROTOBUFDRIVEMODEDIRECT_H
#define PROTOBUFDRIVEMODEDIRECT_H
#include <memory>
#include "interface/IfceDriveMode.h"

namespace Orion {


    class ProtobufDriveModeDirect : public IfceDriveMode
    {
        class DriveModeDirectPrivate;
        friend class DriveModeDirectPrivate;
    public:
        ProtobufDriveModeDirect();
        ~ProtobufDriveModeDirect();

        void processInput(const QByteArray &rawData) override;
        double getValue(int wheel) override;

    private:
        std::unique_ptr<DriveModeDirectPrivate> priv;
    };

}
#endif // PROTOBUFDRIVEMODEDIRECT_H
