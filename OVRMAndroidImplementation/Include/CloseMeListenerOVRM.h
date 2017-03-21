#ifndef ANDROIDIMPLEMENTATION_CLOSEMELISTENEROVRM_H
#define ANDROIDIMPLEMENTATION_CLOSEMELISTENEROVRM_H

#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <assert.h>

namespace cl
{
class CloseMeListenerOVRM : public EventGazeListener
{
  public:
    CloseMeListenerOVRM(ILoggerFactory *loggerFactory);
    void onGazeStarted();
    void onGazeEnded();
    void onGaze();
    bool inFocus();

  private:
    std::unique_ptr<ILogger> logger;
    std::string tag = "CloseMeListenerOVRM";
    bool focus;
};
}

#endif //ANDROIDIMPLEMENTATION_CLOSEMELISTENEROVRM_H