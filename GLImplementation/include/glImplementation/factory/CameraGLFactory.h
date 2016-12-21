#ifndef GLIMPLEMENTATION_CAMERAGLFACTORY_H
#define GLIMPLEMENTATION_CAMERAGLFACTORY_H

#include <coreEngine/factory/ICameraFactory.h>
#include <glImplementation/renderObjects/CameraGL.h>

namespace cl{
    class CameraGLFactory : public ICameraFactory{
    public:
        CameraGLFactory(ILoggerFactory *loggerFactory){
            this->loggerFactory = loggerFactory;
        }
        std::unique_ptr<Camera> create(const std::string &sceneId, Scene *scene){
            return std::unique_ptr<Camera>(new CameraGL(sceneId, loggerFactory, scene));
        }
    private:
        ILoggerFactory *loggerFactory;
    };
}

#endif //GLIMPLEMENTATION_CAMERAGLFACTORY_H