#ifndef COREENGINE_TRANSFORMMODELFACTORY_H
#define COREENGINE_TRANSFORMMODELFACTORY_H

#include <memory>
#include <coreEngine/components/transform/ITransformModelFactory.h>

namespace cl{
    class TransformModelFactory : public ITransformModelFactory{
    public:
        TransformModelFactory(ILoggerFactory *loggerFactory){
            this->loggerFactory = loggerFactory;
        }
        std::unique_ptr<TransformModel> create(Model *model){
            return std::unique_ptr<TransformModel>(new TransformModel(model, loggerFactory));
        }

    private:
        ILoggerFactory *loggerFactory;
    };
}

#endif //COREENGINE_TRANSFORMMODELFACTORY_H