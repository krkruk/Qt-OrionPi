#ifndef JSONCHASSISFEEDBACKGENERATOR_H
#define JSONCHASSISFEEDBACKGENERATOR_H
#include <memory>
#include "interface/IfceChassisFeedbackGenerator.h"


namespace Orion {
    class JsonChassisFeedbackGenerator : public IfceChassisFeedbackGenerator
    {
        class JsonGeneratorPrivate;
        friend class JsonGeneratorPrivate;
    public:
        JsonChassisFeedbackGenerator();
        ~JsonChassisFeedbackGenerator();

        void addInput(QWeakPointer<IfceWheelModel> model) override;

        /**
         * @brief generate Generates feedback information in JsonFormat.
         *
         * The format is as follows:
         * {
         *  DRIVE_GROUP_KEY: [
         *      "numerical_id1": {
         *          ANGULAR_VELOCITY_KEY: value(deg/s),
         *          CURRENT_KEY: value(ampers),
         *          HEATSINK_TEMP_KEY: value(centigrade),
         *          PWM_KEY: value(integer),
         *          ERROR_CODE_KEY: value(integer)
         *      },
         *      "numerical_id2": {
         *          ANGULAR_VELOCITY_KEY: value(deg/s),
         *          CURRENT_KEY: value(ampers),
         *          HEATSINK_TEMP_KEY: value(centigrade),
         *          PWM_KEY: value(integer),
         *          ERROR_CODE_KEY: value(integer)
         *      },
         *      ...
         *  ]
         * }
         * @return
         */
        QByteArray generate() override;

        /**
         * @brief clear Clears weak references to IfceWheelModels
         */
        void clear();

    private:
        std::unique_ptr<JsonGeneratorPrivate> priv;
    };
}

#endif // JSONCHASSISFEEDBACKGENERATOR_H
