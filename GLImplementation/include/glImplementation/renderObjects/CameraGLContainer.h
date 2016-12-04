#ifndef GLIMPLEMENTATION_CAMERAGLCONTAINER_H
#define GLIMPLEMENTATION_CAMERAGLCONTAINER_H

#include <memory>
#include <coreEngine/renderObjects/IRenderable.h>
#include <glImplementation/renderObjects/CameraGLContainer.h>
#include <conf/GLTypes.h>

namespace cl{
    class CameraGLContainer : public IRenderable{
    public:
        CameraGLContainer(std::unique_ptr<CL_GLuint> viewMatrixId, std::unique_ptr<CL_GLuint> projectionMatrixId, CameraGL *cameraGL);
        bool initialize();
        void draw();
        void deinitialize();
    };
}

#endif //GLIMPLEMENTATION_CAMERAGLCONTAINER_H