#ifndef ANDROIDIMPLEMENTATION_ACTIONBUTTONLISTENERGVR_H
#define ANDROIDIMPLEMENTATION_ACTIONBUTTONLISTENERGVR_H

#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <assert.h>

namespace cl
{
class ActionButtonListenerGVR : public EventGazeListener
{
public:
  ActionButtonListenerGVR(ILoggerFactory *loggerFactory);
  void onGazeStarted();
  void onGazeEnded();
  void onGaze();
  bool inFocus();

private:
  std::unique_ptr<ILogger> logger;
  std::string tag = "ActionButtonListenerGVR";
  bool focus;
};
}

#endif //ANDROIDIMPLEMENTATION_ACTIONBUTTONLISTENERGVR_H