#ifndef RENDERER_RENDERERNOHMDSTEREO_H
#define RENDERER_RENDERERNOHMDSTEREO_H

#include <coreEngine/IRenderer.h>
#include <renderer/RendererNoHMD.h>

namespace cl {
class RendererNoHMDStereo : public RendererNoHMD {
 public:
  void drawInit(Scene *scene);
  void drawLeft(Scene *scene);
  void drawRight(Scene *scene);
  void drawComplete();

 private:
  void drawScene(Scene *scene);
};
}

#endif  // RENDERER_RENDERERNOHMD_H