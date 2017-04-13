#ifndef ANDROIDIMPLEMENTATION_NOTIFYMELISTENEROVRM_H
#define ANDROIDIMPLEMENTATION_NOTIFYMELISTENEROVRM_H

#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <assert.h>

namespace cl
{
class NotifyMeListenerOVRM : public EventGazeListener
{
  public:
    NotifyMeListenerOVRM(ILoggerFactory *loggerFactory);
    void onGazeStarted();
    void onGazeEnded();
    void onGaze();
    bool inFocus();

  private:
    std::unique_ptr<ILogger> logger;
    std::string tag = "NotfiyMeListenerOVRM";
    bool focus;
};
}

#endif //ANDROIDIMPLEMENTATION_NOTIFYMELISTENEROVRM_H