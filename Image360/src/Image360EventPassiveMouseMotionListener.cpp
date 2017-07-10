#include <image360/Image360EventPassiveMouseMotionListener.h>

namespace cl {
Image360EventPassiveMouseMotionListener::
    Image360EventPassiveMouseMotionListener(Image360 *image360,
                                            ILoggerFactory *loggerFactory) {
  assert(image360 != nullptr);
  this->image360 = image360;
  this->logger =
      loggerFactory->createLogger("Image360:EventPassiveMouseMotionListener::");
  return;
}
void Image360EventPassiveMouseMotionListener::onPassiveMouseMotion(int x,
                                                                   int y) {
  // logger->log(LOG_DEBUG, "Mouse move:" + std::to_string(x) + "," +
  // std::to_string(y));
  if (this->lastPassiveMousePositionX != -1) {
    float xoff = (x - this->lastPassiveMousePositionX) *
                 this->passiveMouseMotionSensitivity;
    float yoff = (y - this->lastPassiveMousePositionY) *
                 this->passiveMouseMotionSensitivity;

    // TransformTreeCamera *transform =
    //     (TransformTreeCamera *)camera->getComponentList().getComponent(
    //         "transformTree");
    // TODO : Error handlings
    TransformTreeModel *transform =
        (TransformTreeModel *)this->image360->getControllerModel()
            ->getComponentList()
            .getComponent("transformTree");
    CL_Vec3 rotation = transform->getLocalRotation();
    // transform->setLocalRotation(
    //     CL_Vec3(rotation.x - yoff, rotation.y - xoff, rotation.z));
    this->image360->updateControllerRotation(
        CL_Vec3(rotation.x - yoff, rotation.y - xoff, rotation.z));
  }
  lastPassiveMousePositionX = x;
  lastPassiveMousePositionY = y;
}
}