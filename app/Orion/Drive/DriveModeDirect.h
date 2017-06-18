#ifndef DRIVEMODEDIRECT_H
#define DRIVEMODEDIRECT_H
#include <QByteArray>
#include <QScopedPointer>
#include "IfceDriveMode.h"


namespace Orion {

    class DriveModeDirect : public IfceDriveMode
    {
        class DriveModeDirectPrivate;
    public:
        DriveModeDirect();
        ~DriveModeDirect();

        void processInput(const QByteArray &rawData) override;
        double getValue(int wheel) override;


    private:
        QScopedPointer<DriveModeDirectPrivate> member;
    };

}
#endif // DRIVEMODEDIRECT_H
