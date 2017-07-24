/*
* This Class contains implementation for a key press listener
* event. This will be used for keyboard events for a windows implementation
*/

#ifndef IMAGE360_EVENTMOUSELISTENER_H
#define IMAGE360_EVENTMOUSELISTENER_H

#include <coreEngine/events/EventMouseListener.h>
#include <coreEngine/util/ILogger.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <image360/Buttons.h>
#include <image360/Image360.h>

/*
* Mouse Button Click Event Handlers for Buttons object
*/
namespace cl {
class Image360EventMouseListener : public EventMouseListener {
 public:
  Image360EventMouseListener(Buttons &buttons, ILoggerFactory &loggerFactory);
  void onLeftMouseUp();
  void onMiddleMouseUp();
  void onRightMouseUp();
  void onLeftMouseDown();
  void onMiddleMouseDown();
  void onRightMouseDown();

 private:
  // Image360 *image360;
  Buttons *buttons;
  std::unique_ptr<ILogger> logger;
};
}

#endif  // IMAGE360_EVENTMOUSELISTENER_H