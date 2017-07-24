#ifndef IMAGE360_FPSCAMERA_H
#define IMAGE360_FPSCAMERA_H

#include <coreEngine/components/gazeDetector/GazeDetectorFactory.h>
#include <coreEngine/components/transformTree/ITransformTreeFactory.h>
#include <coreEngine/factory/ICameraFactory.h>
#include <coreEngine/renderObjects/Camera.h>
#include <coreEngine/renderObjects/Scene.h>
#include <coreEngine/util/ILogger.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <image360/ApplicationObject.h>

namespace cl {
class FPSCamera : public ApplicationObject {
 public:
  FPSCamera(ILoggerFactory &loggerFactory,
            ITransformTreeFactory &transformTreeFactory,
            ICameraFactory &cameraFactory);
  void initialize(Scene &scene);
  TransformTree *getCameraTransformTree() { return this->gazeTransformShooter; }

 private:
  Camera *camera;
  std::unique_ptr<Camera> cameraUptr;
  // TODO: maybe this should be called the gazeTransformSource??
  TransformTree *gazeTransformShooter;

  std::unique_ptr<ILogger> logger;
  ICameraFactory *cameraFactory;
  ITransformTreeFactory *transformTreeFactory;

  // constants
  float ASPECT_RATIO = 1.5f;
  float NEAR_PLANE = 0.1f;
  float FAR_PLANE = 1000.0f;
  float FOV = 1.75f;
  CL_Vec3 CAMERA_POSITION = CL_Vec3(0.0f, 0.0f, 0.0f);
  CL_Vec3 CAMERA_ROTATION = CL_Vec3(0.0f, 0.0f, 0.0f);
};
}

#endif  // IMAGE360_FPSCAMERA_H