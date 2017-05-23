#ifndef DAYDREAMIMPLEMENTATION_CLOSEBUTTONLISTENER_H
#define DAYDREAMIMPLEMENTATION_CLOSEBUTTONLISTENER_H

#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <assert.h>

namespace cl
{
class CloseButtonListenerDaydream : public EventGazeListener
{
public:
  CloseButtonListenerDaydream(ILoggerFactory *loggerFactory);
  void onGazeStarted();
  void onGazeEnded();
  void onGaze();
  bool inFocus();

private:
  std::unique_ptr<ILogger> logger;
  std::string tag = "CloseButtonListenerDaydream";
  bool focus = false;
};
}

#endif //DAYDREAMIMPLEMENTATION_CLOSEBUTTONLISTENER_H