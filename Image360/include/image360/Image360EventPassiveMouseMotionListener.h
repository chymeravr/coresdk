/*
* This Class contains implementation for a passive motion motion listener
* event. This will be used for mouse movement for a windows implementation
*/

#ifndef IMAGE360_EVENTPASSIVEMOUSEMOTIONLISTENER_H
#define IMAGE360_EVENTPASSIVEMOUSEMOTIONLISTENER_H

#include <coreEngine/events/EventPassiveMouseMotionListener.h>
#include <coreEngine/util/ILogger.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <image360/FPSCamera.h>
#include <image360/Image360.h>

// this class should probably have a better name
namespace cl {
class Image360EventPassiveMouseMotionListener
    : public EventPassiveMouseMotionListener {
 public:
  Image360EventPassiveMouseMotionListener(FPSCamera &fpsCamera,
                                          ILoggerFactory &loggerFactory);
  void onPassiveMouseMotion(int x, int y);

 private:
  FPSCamera *fpsCamera;
  std::unique_ptr<ILogger> logger;

  int lastPassiveMousePositionX = -1;
  int lastPassiveMousePositionY = -1;
  float passiveMouseMotionSensitivity = 0.35f;
};
}

#endif  // IMAGE360_EVENTPASSIVEMOUSEMOTIONLISTENER_H