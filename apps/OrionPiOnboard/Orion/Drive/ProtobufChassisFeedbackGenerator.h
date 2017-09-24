#ifndef PROTOBUFCHASSISFEEDBACKGENERATOR_H
#define PROTOBUFCHASSISFEEDBACKGENERATOR_H
#include <memory>
#include "interface/IfceChassisFeedbackGenerator.h"

namespace Orion {


    class ProtobufChassisFeedbackGenerator : public IfceChassisFeedbackGenerator
    {
        class FeedbackGeneratorPrivate;
        friend class FeedbackGeneratorPrivate;

    public:
        ProtobufChassisFeedbackGenerator();
        ~ProtobufChassisFeedbackGenerator();

        void addInput(QWeakPointer<IfceWheelModel> model) override;
        QByteArray generate() override;
        void clear() override;


    private:
        std::unique_ptr<FeedbackGeneratorPrivate> priv;
    };

}
#endif // PROTOBUFCHASSISFEEDBACKGENERATOR_H
