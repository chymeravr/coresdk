#include <image360/Image360EventMouseListener.h>

namespace cl {
Image360EventMouseListener::Image360EventMouseListener(
    Buttons &buttons, ILoggerFactory &loggerFactory) {
  this->buttons = &buttons;
  this->logger = loggerFactory.createLogger("Image360:EventMouseListener:: ");
}
void Image360EventMouseListener::onLeftMouseUp() {}
void Image360EventMouseListener::onMiddleMouseUp() {}
void Image360EventMouseListener::onRightMouseUp() {}

void Image360EventMouseListener::onLeftMouseDown() {
  logger->log(LOG_DEBUG, "LeftMouseDown");
  this->buttons->onClickHandler();
}
void Image360EventMouseListener::onMiddleMouseDown() {}
void Image360EventMouseListener::onRightMouseDown() {
  logger->log(LOG_DEBUG, "RightMouseDown");
}
}