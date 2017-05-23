#ifndef DAYDREAMIMPLEMENTATION_ACTIONBUTTONLISTENER_H
#define DAYDREAMIMPLEMENTATION_ACTIONBUTTONLISTENER_H

#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <assert.h>

namespace cl
{
class ActionButtonListenerDaydream : public EventGazeListener
{
public:
  ActionButtonListenerDaydream(ILoggerFactory *loggerFactory);
  void onGazeStarted();
  void onGazeEnded();
  void onGaze();
  bool inFocus();

private:
  std::unique_ptr<ILogger> logger;
  std::string tag = "ActionButtonListenerDaydream";
  bool focus;
};
}

#endif //DAYDREAMIMPLEMENTATION_ACTIONBUTTONLISTENER_H