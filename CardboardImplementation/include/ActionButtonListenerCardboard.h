#ifndef CARDBOARDIMPLEMENTATION_ACTIONBUTTONLISTENER_H
#define CARDBOARDIMPLEMENTATION_ACTIONBUTTONLISTENER_H

#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <assert.h>

namespace cl
{
class ActionButtonListenerCardboard : public EventGazeListener
{
public:
  ActionButtonListenerCardboard(ILoggerFactory *loggerFactory);
  void onGazeStarted();
  void onGazeEnded();
  void onGaze();
  bool inFocus();

private:
  std::unique_ptr<ILogger> logger;
  std::string tag = "ActionButtonListenerCardboard";
  bool focus;
};
}

#endif //CARDBOARDIMPLEMENTATION_ACTIONBUTTONLISTENER_H