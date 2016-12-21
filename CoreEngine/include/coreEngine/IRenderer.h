#ifndef COREENGINE_IRENDERER_H
#define COREENGINE_IRENDERER_H

#include<coreEngine/renderObjects/Scene.h>

namespace cl{
    class IRenderer{
    public:
        virtual bool start() = 0;
        virtual bool initialize(Scene *scene) = 0;
        virtual void update() = 0;
        virtual void draw(Scene *scene) = 0;
        virtual void deinitialize(Scene *scene) = 0;
        virtual void stop() = 0;
    };
}

#endif //COREENGINE_IRENDERER_H