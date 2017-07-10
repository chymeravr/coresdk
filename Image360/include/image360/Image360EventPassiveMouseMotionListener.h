/*
* This Class contains implementation for a passive motion motion listener
* event. This will be used for mouse movement for a windows implementation
*/

#ifndef IMAGE360_EVENTPASSIVEMOUSEMOTIONLISTENER_H
#define IMAGE360_EVENTPASSIVEMOUSEMOTIONLISTENER_H

#include <coreEngine/events/EventPassiveMouseMotionListener.h>
#include <coreEngine/util/ILogger.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <image360/Image360.h>

namespace cl {
class Image360EventPassiveMouseMotionListener
    : public EventPassiveMouseMotionListener {
 public:
  Image360EventPassiveMouseMotionListener(Image360 *image360,
                                          ILoggerFactory *loggerFactory);
  void onPassiveMouseMotion(int x, int y);

 private:
  Image360 *image360;
  std::unique_ptr<ILogger> logger;

  int lastPassiveMousePositionX = -1;
  int lastPassiveMousePositionY = -1;
  float passiveMouseMotionSensitivity = 0.35f;
};
}

#endif  // IMAGE360_EVENTPASSIVEMOUSEMOTIONLISTENER_H