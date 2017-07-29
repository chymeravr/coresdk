#ifndef IMAGE360_CAMERARETICLE_H
#define IMAGE360_CAMERARETICLE_H

#include <coreEngine/renderObjects/Scene.h>
#include <coreEngine/ui/UIFactory.h>
#include <image360/ApplicationObject.h>

/*
    CameraReticle defines a reticle (crosshair) that is attached to the
    camera and moves in sync with it (think of it like a aim sight for a gun
    in a FPS games)
*/

namespace cl {
class CameraReticle : public ApplicationObject {
 public:
  CameraReticle(ILoggerFactory &loggerFactory, UIFactory &uiFactory,
                TransformTree &transformTreeCamera);
  ~CameraReticle();
  void initialize(Scene &scene);

 private:
  std::unique_ptr<Reticle> reticle;
  std::unique_ptr<ILogger> logger;
  UIFactory *uiFactory;
  TransformTree *transformTreeCamera;

  CL_Vec4 CAMERA_RETICLE_COLOR = CL_Vec4(0.0f, 1.0f, 0.0f, 1.0f);
};
}

#endif  // IMAGE360_CAMERARETICLE_H