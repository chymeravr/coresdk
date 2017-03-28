#ifndef ANDROIDIMPLEMENTATION_NOTIFYMELISTENERGVR_H
#define ANDROIDIMPLEMENTATION_NOTIFYMELISTENERGVR_H

#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <assert.h>

namespace cl
{
class NotifyMeListenerGVR : public EventGazeListener
{
public:
  NotifyMeListenerGVR(ILoggerFactory *loggerFactory);
  void onGazeStarted();
  void onGazeEnded();
  void onGaze();
  bool inFocus();

private:
  std::unique_ptr<ILogger> logger;
  std::string tag = "NotifyMeListenerGVR";
  bool focus;
};
}

#endif //ANDROIDIMPLEMENTATION_NOTIFYMELISTENERGVR_H