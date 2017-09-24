#ifndef WHEELMODEL_H
#define WHEELMODEL_H
#include <memory>

#include "IfceWheelModel.h"

namespace Orion {
    class WheelModel : public IfceWheelModel
    {
        class WheelModelPrivate;
        std::unique_ptr<WheelModelPrivate> member;
    public:
        WheelModel(int id);
        ~WheelModel();

        void updateModel(const QByteArray &line) override;
        void addObserver(QWeakPointer<IfceSerialObserver> observer) override;
        bool delObserver(QWeakPointer<IfceSerialObserver> observer) override;

        /**
         * @brief notifyObservers Notifies observers that internal state
         * of the model has changed (usually, expectedAngularVelocity is changed)
         *
         * Notifies observers. Creates a message suitable
         * for {@link IfceSerialModel}.
         */
        void notifyObservers() override;

        void updateAngularVelocity(double angularVelocity) override;
        double getExpectedAngularVelocity() const override;

        double getCurrentAngularVelocity() const override;
        double getCurrent() const override;
        double getHeatSinkTemperature() const override;
        int getPwm() const override;
        int getErrorCode() const override;
    };

}

#endif // WHEELMODEL_H
