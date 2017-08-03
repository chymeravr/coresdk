#ifndef IMAGE360_CLOSEBUTTONLISTENER_H
#define IMAGE360_CLOSEBUTTONLISTENER_H

#include <assert.h>
#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <coreEngine/util/ILoggerFactory.h>

namespace cl {
class CloseButtonListener : public EventGazeListener {
 public:
  CloseButtonListener(ILoggerFactory *loggerFactory) {
    assert(loggerFactory != nullptr);
    this->logger = loggerFactory->createLogger(tag);
  }
  void onGazeStarted() {
    logger->log(LOG_INFO, "CloseMe: OnGazeStarted");
    this->focus = true;
  }
  void onGazeEnded() {
    logger->log(LOG_INFO, "CloseMe: OnGazeEnded");
    this->focus = false;
  }
  void onGaze() {
    // logger->log(LOG_INFO, "CloseMe: OnGaze");
  }
  bool inFocus() { return this->focus; }

 private:
  std::unique_ptr<ILogger> logger;
  std::string tag = "CloseButtonListener";
  bool focus = false;
};
}

#endif  // IMAGE360_CLOSEBUTTONLISTENER_H