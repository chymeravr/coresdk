#include <image360/Image360EventPassiveMouseMotionListener.h>

namespace cl {
Image360EventPassiveMouseMotionListener::
    Image360EventPassiveMouseMotionListener(FPSCamera &fpsCamera,
                                            ILoggerFactory &loggerFactory)
    : EventPassiveMouseMotionListener() {
  // assert(image360 != nullptr);
  this->fpsCamera = &fpsCamera;
  this->logger =
      loggerFactory.createLogger("Image360:EventPassiveMouseMotionListener::");
  return;
}
void Image360EventPassiveMouseMotionListener::onPassiveMouseMotion(int x,
                                                                   int y) {
  if (this->lastPassiveMousePositionX != -1) {
    float xoff = (x - this->lastPassiveMousePositionX) *
                 this->passiveMouseMotionSensitivity;
    float yoff = (y - this->lastPassiveMousePositionY) *
                 this->passiveMouseMotionSensitivity;

    // TODO : Error handlings
    TransformTree *transform = this->fpsCamera->getCameraTransformTree();
    // this->image360->getFPSCamera()->getCameraTransformTree();
    CL_Vec3 rotation = transform->getLocalRotation();
    // TransformTreeModel *transform =
    //     (TransformTreeModel *)this->image360->getControllerModel()
    //         ->getComponentList()
    //         .getComponent("transformTree");
    // CL_Vec3 rotation = transform->getLocalRotation();
    transform->setLocalRotation(
        CL_Vec3(rotation.x - yoff, rotation.y - xoff, rotation.z));

    // this->image360->updateControllerRotation(
    //     CL_Vec3(rotation.x - yoff, rotation.y - xoff, rotation.z));
  }
  lastPassiveMousePositionX = x;
  lastPassiveMousePositionY = y;
}
}