//
// Created by robin_chimera on 1/9/2017.
//

#ifndef GRAPHICSSDK_CAMERAGLOVR_H
#define GRAPHICSSDK_CAMERAGLOVR_H

#include <coreEngine/renderObjects/Camera.h>
#include <coreEngine/renderObjects/IRenderable.h>

namespace cl{
    class CameraGLOVR : public Camera, public IRenderable {
    public:
        CameraGLOVR(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene);
        IRenderable *getRenderable();
        bool initialize();
        void draw();
        void deinitialize();
        void setViewMatrix(CL_Mat44 viewMatrix);
        void setProjectionMatrix(CL_Mat44 viewMatrix);
        void calculateViewMatrix();
        void calculateProjectionMatrix();
    private:
        std::unique_ptr<ILogger> logger;
    };
}

#endif //GRAPHICSSDK_CAMERAGLOVR_H
