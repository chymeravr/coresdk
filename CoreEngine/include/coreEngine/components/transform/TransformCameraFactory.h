#ifndef COREENGINE_TRANSFORMCAMERAFACTORY_H
#define COREENGINE_TRANSFORMCAMERAFACTORY_H

#include <memory>
#include <coreEngine/components/transform/ITransformCameraFactory.h>

namespace cl{
    class TransformCameraFactory : public ITransformCameraFactory{
    public:
        TransformCameraFactory(ILoggerFactory *loggerFactory){
            this->loggerFactory = loggerFactory;
        }
        std::unique_ptr<TransformCamera> create(Camera *camera){
            return std::unique_ptr<TransformCamera>(new TransformCamera(camera, loggerFactory));
        }

    private:
        ILoggerFactory *loggerFactory;
    };
}

#endif //COREENGINE_TRANSFORMCAMERAFACTORY_H