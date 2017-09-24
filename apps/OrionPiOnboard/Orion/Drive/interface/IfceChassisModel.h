#ifndef IFCECHASSISMODEL_H
#define IFCECHASSISMODEL_H
#include <QSharedPointer>

namespace Orion {
    class IfceWheelModel;
    class IfceDriveMode;
    class IfceChassisFeedbackGenerator;

    class IfceChassisModel
    {
    public:
        ~IfceChassisModel() {}

        /**
         * @brief setDriveAlgorithm Sets an algorithm that is responsible for
         * processing remote data that is to be applied for all the wheels.
         * @param algorithm
         */
        virtual void setDriveAlgorithm(QSharedPointer<IfceDriveMode> algorithm) = 0;

        /**
         * @brief setFeedbackGeneratorAlgorithm
         * @param generator
         */
        virtual void setFeedbackGeneratorAlgorithm(QSharedPointer<IfceChassisFeedbackGenerator> generator) = 0;

        /**
         * @brief addWheel Adds a wheel to the model. Each wheel
         * requires a unique ID.
         * @param wheel
         */
        virtual void addWheel(QSharedPointer<IfceWheelModel> wheel) = 0;

        /**
         * @brief updateState Updates a desired state of the chassis. The method
         * requires data from the remote source in a specified data format.
         * @param data
         */
        virtual void updateState(const QByteArray &data) = 0;

        /**
         * @brief notifyAll Notifies the remote data was just passed to the chassis model.
         * Calling the method influences hardware serial devices.
         */
        virtual void notifyAll() = 0;

        /**
         * @brief getFeedbackData Generates feedback data in a specified format.
         * @return
         */
        virtual QByteArray getFeedbackData() = 0;
    };
}
#endif // IFCECHASSISMODEL_H
