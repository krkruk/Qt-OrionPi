#ifndef CHASSISMODEL_H
#define CHASSISMODEL_H
#include <QObject>
#include <QSharedPointer>
#include <QHash>
#include "IfceDriveMode.h"
#include "IfceChassisModel.h"
#include "../Network/IfceNetworkReceiver.h"

namespace Orion {
    class IfceWheelModel;


    class ChassisModel : public QObject, public IfceChassisModel, public IfceNetworkReceiver
    {
        Q_OBJECT
    public:
        explicit ChassisModel(QObject *parent = nullptr);
        ~ChassisModel();

        void setDriveAlgorithm(QSharedPointer<IfceDriveMode> algorithm) override;
        void addWheel(QSharedPointer<IfceWheelModel> wheel) override;
        void notifyAll() override;


    public slots:
        void onRemoteDataReceived(const QByteArray &cmd) override;


    private:
        void update_angular_velocities_left_side(double angVelLeftSide);
        void update_angular_velocities_right_side(double angVelRightSide);
        void notify_left_side();
        void notify_right_side();

        using IdToWheelModel = QHash<int, QSharedPointer<IfceWheelModel>>;

        IdToWheelModel leftRowWheels;
        IdToWheelModel rightRowWheels;
        QSharedPointer<IfceDriveMode> driveAlgorithm;
    };
}
#endif // CHASSISMODEL_H
