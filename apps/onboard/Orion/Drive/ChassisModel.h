#ifndef CHASSISMODEL_H
#define CHASSISMODEL_H
#include <QObject>
#include <QSharedPointer>
#include <QHash>
#include "IfceDriveMode.h"
#include "IfceChassisModel.h"


namespace Orion {
    class IfceWheelModel;


    class ChassisModel : public QObject, public IfceChassisModel
    {
    public:
        explicit ChassisModel(QObject *parent = nullptr);
        ~ChassisModel();

        void setDriveAlgorithm(QSharedPointer<IfceDriveMode> algorithm) override;
        void addWheel(QSharedPointer<IfceWheelModel> wheel) override;
        void notifyAll() override;
        void updateState(const QByteArray &cmd) override;

        void setFeedbackGeneratorAlgorithm(QSharedPointer<IfceChassisFeedbackGenerator> generator);
        QByteArray getFeedbackData();

    private:
        void update_angular_velocities_left_side(double angVelLeftSide);
        void update_angular_velocities_right_side(double angVelRightSide);
        void notify_left_side();
        void notify_right_side();

        using IdToWheelModel = QHash<int, QSharedPointer<IfceWheelModel>>;

        IdToWheelModel leftRowWheels;
        IdToWheelModel rightRowWheels;
        QSharedPointer<IfceDriveMode> driveAlgorithm;
        QSharedPointer<IfceChassisFeedbackGenerator> feedbackGenerator;
    };
}
#endif // CHASSISMODEL_H
