#ifndef GLIMPLEMENTATION_SCENEGLFACTORY_H
#define GLIMPLEMENTATION_SCENEGLFACTORY_H

#include <coreEngine/factory/ISceneFactory.h>
#include <glImplementation/renderObjects/SceneGL.h>

namespace cl{
    class SceneGLFactory : public ISceneFactory{
    public:
        SceneGLFactory(ILoggerFactory *loggerFactory){
            this->loggerFactory = loggerFactory;
        }

        std::unique_ptr<Scene> create(std::string id){
            return std::unique_ptr<Scene>(new SceneGL(loggerFactory, id));
        }

    private:
        ILoggerFactory *loggerFactory;
    };
}

#endif //GLIMPLEMENTATION_SCENEGLFACTORY_H