/*
* This Class contains implementation for a passive motion motion listener
* event. This will be used for mouse movement for a windows implementation
*/

#ifndef IMAGE360_EVENTCONTROLLERPOSITIONLISTENER_H
#define IMAGE360_EVENTCONTROLLERPOSITIONLISTENER_H

#include <coreEngine/events/EventControllerPositionListener.h>
#include <coreEngine/util/ILogger.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <image360/Image360.h>
#include <coreEngine/components/transformTree/TransformTree.h>

namespace cl {
class Image360EventControllerPositionListener
    : public EventControllerPositionListener {
 public:
  Image360EventControllerPositionListener(Image360 *image360,
                                          ILoggerFactory *loggerFactory);
  void updateControllerPosition(float pos_x, float pos_y, float pos_z);

 private:
  Image360 *image360;
  std::unique_ptr<ILogger> logger;
};
}

#endif  // IMAGE360_EVENTPASSIVEMOUSEMOTIONLISTENER_H