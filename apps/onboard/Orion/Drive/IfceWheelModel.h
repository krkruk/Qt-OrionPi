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

        /**
         * @brief updateAngularVelocity Sets angular velocity. The value is
         * to be conveyed to a serial device that is responsible for execution
         * of the command in the real world.
         * @param angularVelocity
         */
        virtual void updateAngularVelocity(double angularVelocity) = 0;

        /**
         * @brief getExpectedAngularVelocity Returns value of recently set angular
         * velocity. The value does not indiate the real state of the hardware.
         * It only says what value should be set on the remote device.
         * See also {@link getCurrentAngularVelocity()}.
         * @return
         */
        virtual double getExpectedAngularVelocity() const = 0;

        /**
         * @brief getCurrentAngularVelocity Returns value of current angular
         * velocity that is present in the hardware. It indicates how fast
         * the wheel spin. The value is updated only via {@link IfceSerialModel::updateModel() }
         * @return Degrees per second
         */
        virtual double getCurrentAngularVelocity() const = 0;

        /**
         * @brief getCurrent Returns value of current withdrawal in the hardware device.
         * The value is updated only via {@link IfceSerialModel::updateModel() }
         * @return Amps
         */
        virtual double getCurrent() const = 0;

        /**
         * @brief getHeatSinkTemperature Returns value of temperature that is
         * received from thermal sensor installed on heat sinks.
         * The value is updated only via {@link IfceSerialModel::updateModel() }
         * @return Centigrade
         */
        virtual double getHeatSinkTemperature() const = 0;

        /**
         * @brief getPwm Returns value of PWM that is currently set in the device.
         * Usually the value is unimportant due to PID controller implemented
         * in the device
         * @return [-127..127]
         */
        virtual int getPwm() const = 0;

        /**
         * @brief getErrorCode Returns error code that is returned by the hardware
         * @return Ask the hardware developer...
         */
        virtual int getErrorCode() const = 0;
    };
}
#endif // IFCEWHEELMODEL_H
