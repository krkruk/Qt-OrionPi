#ifndef JSONCHASSISFEEDBACKGENERATOR_H
#define JSONCHASSISFEEDBACKGENERATOR_H
#include <memory>
#include "IfceChassisFeedbackGenerator.h"

namespace Orion {
    class JsonChassisFeedbackGenerator : public IfceChassisFeedbackGenerator
    {
        class JsonGeneratorPrivate;
        friend class JsonGeneratorPrivate;
    public:
        JsonChassisFeedbackGenerator();
        ~JsonChassisFeedbackGenerator();

        void addInput(QWeakPointer<IfceWheelModel> model) override;
        QByteArray generate() override;
        void clear();

    private:
        std::unique_ptr<JsonGeneratorPrivate> priv;
    };
}

#endif // JSONCHASSISFEEDBACKGENERATOR_H
