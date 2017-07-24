#ifndef IMAGE360_EVENTBEGINFADE_H
#define IMAGE360_EVENTBEGINFADE_H

#include <coreEngine/events/IEvent.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <image360/FadeScreen.h>

namespace cl {
class Image360EventBeginFade : public IEvent {
 public:
  Image360EventBeginFade(FadeScreen &fadeScreen, ILoggerFactory &loggerFactory) {
    this->fadeScreen = &fadeScreen;
    this->logger = loggerFactory.createLogger("Image360EventBeginFade:");
  }
  void callListener() {
    this->logger->log(LOG_DEBUG, "Begin Fade Listener Call");
    this->fadeScreen->beginFade(); 
    }

 private:
  FadeScreen *fadeScreen;
  std::unique_ptr<ILogger> logger;
};
}

#endif  // IMAGE360_EVENTBEGINFADE_H