#include <image360/Image360EventControllerPositionListener.h>

namespace cl {
Image360EventControllerPositionListener::
    Image360EventControllerPositionListener(Image360 *image360,
                                            ILoggerFactory *loggerFactory) {
  this->image360 = image360;
  this->logger =
      loggerFactory->createLogger("Image360:EventControllerPosition:: ");
}

void Image360EventControllerPositionListener::updateControllerPosition(
    float pos_x, float pos_y, float pos_z) {
  // TODO : Error handlings
	TransformTree *transform = nullptr;
      /*(TransformTreeModel *)this->image360->getControllerModel()
          ->getComponentList()
          .getComponent("transformTree");*/
  CL_Vec3 position = CL_Vec3(pos_x, pos_y, pos_z);
  /*this->image360->updateControllerRotation(
      CL_Vec3(rotation.x - yoff, rotation.y - xoff, rotation.z));*/
}
}