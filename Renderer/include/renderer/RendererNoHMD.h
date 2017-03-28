#ifndef RENDERER_RENDERERNOHMD_H
#define RENDERER_RENDERERNOHMD_H

#include <coreEngine/IRenderer.h>

namespace cl
{
class RendererNoHMD : public IRenderer
{
  public:
    bool start();
    bool initialize(Scene *scene);
    void update();
    void drawInit(Scene *scene);
    void draw(Scene *scene, EYE eye);
    void drawComplete();
    void deinitialize(Scene *scene);
    void stop();
    void resume();
    void pause();
};
}

#endif //RENDERER_RENDERERNOHMD_H