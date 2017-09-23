#ifndef JSONDRIVEMODEDIRECT_H
#define JSONDRIVEMODEDIRECT_H
#include <QByteArray>
#include <QScopedPointer>
#include "IfceDriveMode.h"


namespace Orion {

    class JsonDriveModeDirect : public IfceDriveMode
    {
        class DriveModeDirectPrivate;
    public:
        JsonDriveModeDirect();
        ~JsonDriveModeDirect();

        void processInput(const QByteArray &rawData) override;
        double getValue(int wheel) override;


    private:
        QScopedPointer<DriveModeDirectPrivate> member;
    };

}
#endif // JSONDRIVEMODEDIRECT_H
