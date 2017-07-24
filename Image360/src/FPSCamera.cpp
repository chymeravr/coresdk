#include <image360/FPSCamera.h>

namespace cl {
FPSCamera::FPSCamera(ILoggerFactory &loggerFactory,
                     ITransformTreeFactory &transformTreeFactory,
                     ICameraFactory &cameraFactory) {
  this->logger = loggerFactory.createLogger("FPSCamera: ");
  this->cameraFactory = &cameraFactory;
  this->transformTreeFactory = &transformTreeFactory;

  this->cameraUptr = this->cameraFactory->create("camera");
  this->camera = cameraUptr.get();

  // camera transforms
  std::unique_ptr<TransformTreeCamera> transformTreeCameraUptr =
      this->transformTreeFactory->createTransformTreeCamera(this->camera);
  this->camera->getComponentList().addComponent(
      std::move(transformTreeCameraUptr));

  TransformTreeCamera *transformTreeCamera =
      (TransformTreeCamera *)this->camera->getComponentList().getComponent(
          "transformTree");
  transformTreeCamera->setLocalPosition(CAMERA_POSITION);
  transformTreeCamera->setLocalRotation(CAMERA_ROTATION);
  this->gazeTransformShooter = transformTreeCamera;
}

void FPSCamera::initialize(Scene &scene) {
  assert(cameraUptr != nullptr);
  this->cameraUptr->setAspect(ASPECT_RATIO);
  this->cameraUptr->setFarPlane(FAR_PLANE);
  this->cameraUptr->setFov(FOV);
  this->cameraUptr->setNearPlane(NEAR_PLANE);

  scene.addToScene(std::move(cameraUptr));
  scene.createBiRelation(this->camera);
  // this->createBiRelation(scene);
}
}