/*
* This Class contains implementation for a key press listener
* event. This will be used for keyboard events for a windows implementation
*/

#ifndef IMAGE360_EVENTKEYPRESSLISTENER_H
#define IMAGE360_EVENTKEYPRESSLISTENER_H

#include <coreEngine/events/EventKeyPressListener.h>
#include <coreEngine/util/ILogger.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <image360/Image360.h>

/*
 * Implementation of Key Press Listener for Image360 Application
*/
namespace cl {
class Image360EventKeyPressListener : public EventKeyPressListener {
 public:
  Image360EventKeyPressListener(Image360 *image360,
                                ILoggerFactory *loggerFactory);
  void onKeyPress(char key, int x, int y);

 private:
  Image360 *image360;
  std::unique_ptr<ILogger> logger;

  float upKeyPressSensitivity = 7.5f;
  float sideKeyPressSensitivity = 10.0f;

  float movementSensitivity = 1.0f;
};
}

#endif  // IMAGE360_EVENTKEYPRESSLISTENER_H