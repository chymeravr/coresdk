#ifndef ANDROIDIMPLEMENTATION_CLOSEBUTTONLISTENERGVR_H
#define ANDROIDIMPLEMENTATION_CLOSEBUTTONLISTENERGVR_H

#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <assert.h>

namespace cl
{
class CloseButtonListenerGVR : public EventGazeListener
{
public:
  CloseButtonListenerGVR(ILoggerFactory *loggerFactory);
  void onGazeStarted();
  void onGazeEnded();
  void onGaze();
  bool inFocus();

private:
  std::unique_ptr<ILogger> logger;
  std::string tag = "CloseButtonListenerGVR";
  bool focus = false;
};
}

#endif //ANDROIDIMPLEMENTATION_CLOSEBUTTONLISTENERGVR_H