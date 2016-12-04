#ifndef COREENGINE_TRANSFORMCAMERA_H
#define COREENGINE_TRANSFORMCAMERA_H

#include <coreEngine/renderObjects/Camera.h>
#include <coreEngine/components/transform/Transform.h>
#include <coreEngine/renderObjects/IRenderable.h>

namespace cl{
    class TransformCamera : public Transform, public IRenderable{
    public:
        TransformCamera(Camera *camera);
        virtual IRenderable *getRenderable();
        virtual bool initialize();
        virtual void draw();
        virtual void deinitialize();
    };
}

#endif //COREENGINE_TRANSFORMCAMERA_H