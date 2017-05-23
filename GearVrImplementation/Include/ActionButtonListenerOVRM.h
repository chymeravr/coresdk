#ifndef ANDROIDIMPLEMENTATION_ACTIONBUTTONLISTENEROVRM_H
#define ANDROIDIMPLEMENTATION_ACTIONBUTTONLISTENEROVRM_H

#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <assert.h>

namespace cl
{
class ActionButtonListenerOVRM : public EventGazeListener
{
public:
  ActionButtonListenerOVRM(ILoggerFactory *loggerFactory);
  void onGazeStarted();
  void onGazeEnded();
  void onGaze();
  bool inFocus();

private:
  std::unique_ptr<ILogger> logger;
  std::string tag = "NotfiyButtonListenerOVRM";
  bool focus;
};
}

#endif //ANDROIDIMPLEMENTATION_ACTIONBUTTONLISTENEROVRM_H