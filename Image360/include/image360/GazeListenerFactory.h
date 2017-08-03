#ifndef IMAGE360_GAZELISTENERFACTORY_H
#define IMAGE360_GAZELISTENERFACTORY_H

#include <coreEngine/factory/IEventGazeListenerFactory.h>
#include <image360/ActionButtonListener.h>
#include <image360/CloseButtonListener.h>

namespace cl {
class GazeListenerFactory : public IEventGazeListenerFactory {
 public:
  GazeListenerFactory(ILoggerFactory *loggerFactory) {
    assert(loggerFactory != nullptr);
    this->loggerFactory = loggerFactory;
  }
  std::unique_ptr<EventGazeListener> createCloseButtonListener() {
    return std::unique_ptr<EventGazeListener>(
        new CloseButtonListener(loggerFactory));
  }

  std::unique_ptr<EventGazeListener> createActionButtonListener() {
    return std::unique_ptr<EventGazeListener>(
        new ActionButtonListener(loggerFactory));
  }

 private:
  ILoggerFactory *loggerFactory = nullptr;
};
}

#endif  // IMAGE360_GAZELISTENERFACTORY_H