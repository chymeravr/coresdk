#ifndef GLIMPLEMENTATION_CAMERAGL_H
#define GLIMPLEMENTATION_CAMERAGL_H

#include <coreEngine/renderObjects/Camera.h>
#include <coreEngine/renderObjects/IRenderable.h>

namespace cl{
    class CameraGL : public Camera, public IRenderable {
    public:
        CameraGL(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene);
        IRenderable *getRenderable();
        bool initialize();
        void draw();
        void deinitialize();
    private:
        std::unique_ptr<ILogger> logger;
    };
}

#endif //GLIMPLEMENTATION_CAMERAGL_H