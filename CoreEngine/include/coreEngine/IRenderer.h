#ifndef COREENGINE_IRENDERER_H
#define COREENGINE_IRENDERER_H

#include <coreEngine/renderObjects/Scene.h>

namespace cl {
// enum EYE
// {
//     LEFT = 0,
//     RIGHT = 1
// };

class IRenderer {
 public:
  virtual bool start() = 0;
  virtual bool initialize(Scene *scene) = 0;
  virtual void update() = 0;

  // draw common stuff - clear buffers etc.
  virtual void drawInit(Scene *scene) = 0;
  // draw left/right eye independently
  virtual void drawLeft(Scene *scene) = 0;
  virtual void drawRight(Scene *scene) = 0;
  virtual void drawComplete() = 0;

  virtual void deinitialize(Scene *scene) = 0;
  virtual void stop() = 0;
  virtual void pause() = 0;
  virtual void resume() = 0;
};
}

#endif  // COREENGINE_IRENDERER_H