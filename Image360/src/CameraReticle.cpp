#include <image360/CameraReticle.h>

namespace cl {
CameraReticle::CameraReticle(ILoggerFactory &loggerFactory,
                             UIFactory &uiFactory,
                             TransformTree &transformTreeCamera) {
  this->logger = loggerFactory.createLogger("CameraReticle: ");
  this->uiFactory = &uiFactory;
  this->transformTreeCamera = &transformTreeCamera;
}

void CameraReticle::initialize(Scene &scene) {
  this->reticle = this->uiFactory->createReticle(
      "reticle", &scene, this->transformTreeCamera, this->CAMERA_RETICLE_COLOR);
}

CameraReticle::~CameraReticle() {
  this->logger->log(LOG_DEBUG, "CameraReticle Destructor");
}
}