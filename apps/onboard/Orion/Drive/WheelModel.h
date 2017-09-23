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

        void update(const QByteArray &line) override;
        void addObserver(QWeakPointer<IfceSerialObserver> observer) override;
        bool delObserver(QWeakPointer<IfceSerialObserver> observer) override;

        /**
         * @brief notifyObservers Notifies observers
         *
         * Notifies observers. Creates a message suitable
         * for {@link IfceSerialModel}.
         */
        void notifyObservers() override;

        void updateAngularVelocity(double angularVelocity) override;
        double getCurrentAngularVelocity() const override;
        double getExpectedAngularVelocity() const override;

        double getCurrent() const;
        double getHeatSinkTemperature() const;
        int getPwm() const;
        int getErrorCode() const;
    };

}

#endif // WHEELMODEL_H
