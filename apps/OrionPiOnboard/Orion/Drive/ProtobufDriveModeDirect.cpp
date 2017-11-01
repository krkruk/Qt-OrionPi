#include "ProtobufDriveModeDirect.h"
#include <QByteArray>
#include "protocolEnums.pb.h"
#include "earthBaseToRoverComm.pb.h"


using namespace Orion;

class ProtobufDriveModeDirect::DriveModeDirectPrivate
{
    friend class ProtobufDriveModeDirect;
public:
    ~DriveModeDirectPrivate() = default;
private:
    ORION_COMM::QUERY::InputDevice chassisData;
};


ProtobufDriveModeDirect::ProtobufDriveModeDirect()
    : priv{new DriveModeDirectPrivate}
{
}

ProtobufDriveModeDirect::~ProtobufDriveModeDirect()
{
}

void ProtobufDriveModeDirect::processInput(const QByteArray &rawData)
{
    ORION_COMM::QUERY::InputDevice chassisCmd;
    chassisCmd.ParseFromArray(rawData, rawData.size());
    priv->chassisData = chassisCmd;
}

double ProtobufDriveModeDirect::getValue(int wheel)
{
    /*
     * Left row of wheels is number in 0,2,4... manner (even numbers)
     * Right row of wheels is number in 1,3,5... manner (odd numbers)
     */
    return isLeftRow(wheel)
            ? priv->chassisData.x_axis_0()
            : priv->chassisData.y_axis_0();
}
