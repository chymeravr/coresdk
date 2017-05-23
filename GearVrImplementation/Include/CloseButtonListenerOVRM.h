#ifndef ANDROIDIMPLEMENTATION_CLOSEBUTTONLISTENEROVRM_H
#define ANDROIDIMPLEMENTATION_CLOSEBUTTONLISTENEROVRM_H

#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <assert.h>

namespace cl
{
class CloseButtonListenerOVRM : public EventGazeListener
{
public:
  CloseButtonListenerOVRM(ILoggerFactory *loggerFactory);
  void onGazeStarted();
  void onGazeEnded();
  void onGaze();
  bool inFocus();

private:
  std::unique_ptr<ILogger> logger;
  std::string tag = "CloseButtonListenerOVRM";
  bool focus;
};
}

#endif //ANDROIDIMPLEMENTATION_CLOSEBUTTONLISTENEROVRM_H