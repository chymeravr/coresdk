#ifndef RENDERER_RENDERERNOHMD_H
#define RENDERER_RENDERERNOHMD_H

#include <coreEngine/IRenderer.h>

namespace cl{
    class RendererNoHMD : public IRenderer{
    public:
        bool start();
        bool initialize(Scene *scene);
        void update();
        void draw(Scene *scene);
        void deinitialize(Scene *scene);
        void stop();
    };
}

#endif //RENDERER_RENDERERNOHMD_H