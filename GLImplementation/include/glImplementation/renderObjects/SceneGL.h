#ifndef GLIMPLEMENTATION_SCENEGL_H
#define GLIMPLEMENTATION_SCENEGL_H

#include <coreEngine/renderObjects/Scene.h>
#include <conf/GLTypes.h>

namespace cl{
    class SceneGL : public Scene, public IRenderable{
    public:
        SceneGL(ILoggerFactory *loggerFactory, std::string id);
        IRenderable *getRenderable();
        bool initialize();
        void draw();
        void deinitialize();
    private:
        std::unique_ptr<ILogger> logger;
        GLuint vertexArrayId;
    };
}

#endif //GLIMPLEMENTATION_SCENEGL_H