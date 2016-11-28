#ifndef ANDROIDSDK_ICAMERARENDERER_H
#define ANDROIDSDK_ICAMERARENDERER_H

namespace cl{
    class Camera;
    class ICameraRenderer{
        virtual bool initialize(Camera *cameraPtr) = 0;
        virtual void draw(Camera *cameraPtr) = 0;
        virtual void deinitialize(Camera *cameraPtr) = 0;
    };
}

#endif //ANDROIDSDK_ICAMERARENDERER_H