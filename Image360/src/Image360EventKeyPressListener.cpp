#include <coreEngine/components/transformTree/TransformTreeModel.h>
#include <image360/Image360EventKeyPressListener.h>

// TODO - Ideally I think this should be a windows implementation detail
namespace cl {
Image360EventKeyPressListener::Image360EventKeyPressListener(
    Image360 *image360, ILoggerFactory *loggerFactory)
    : EventKeyPressListener() {
  assert(image360 != nullptr);
  this->image360 = image360;
  this->logger =
      loggerFactory->createLogger("Image360:EventKeyPressListener::");
}
void Image360EventKeyPressListener::onKeyPress(char key, int x, int y) {
  // logger->log(LOG_DEBUG, "Key pressed:" + std::string(1, key));
  logger->log(LOG_DEBUG, "Key pressed:" + std::string(1, key));
  TransformTree *transform = nullptr;//this->image360->getFPSCamera()->getCameraTransformTree();
  // TODO : Error Handling
  CL_Vec3 rot = transform->getLocalRotation();
  CL_Vec3 pos = transform->getLocalPosition();

  switch (key) {
    case 'W':
      rot[0] += this->upKeyPressSensitivity;
      break;
    case 'S':
      rot[0] -= this->upKeyPressSensitivity;
      break;
    case 'A':
      rot[1] += this->sideKeyPressSensitivity;
      break;
    case 'D':
      rot[1] -= this->sideKeyPressSensitivity;
      break;
    case 'I':
      pos[2] += this->movementSensitivity;
      break;
    case 'K':
      pos[2] -= this->movementSensitivity;
      break;
    case 'J':
      pos[0] -= this->movementSensitivity;
      break;
    case 'L':
      pos[0] += this->movementSensitivity;
      break;
    default:
      break;
  }
  transform->setLocalRotation(rot);
  transform->setLocalPosition(pos);
}
}