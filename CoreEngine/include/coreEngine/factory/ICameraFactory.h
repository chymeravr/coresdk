#ifndef COREENGINE_ICAMERAFACTORY_H
#define COREENGINE_ICAMERAFACTORY_H

#include <memory>
#include <coreEngine/renderObjects/Camera.h>

namespace cl{
    class ICameraFactory{
    public:
        virtual std::unique_ptr<Camera> create(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene) = 0;
    };
}

#endif //COREENGINE_ICAMERAFACTORY_H