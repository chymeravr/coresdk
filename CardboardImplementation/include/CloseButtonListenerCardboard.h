#ifndef CARDBOARDIMPLEMENTATION_CLOSEBUTTONLISTENER_H
#define CARDBOARDIMPLEMENTATION_CLOSEBUTTONLISTENER_H

#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <assert.h>

namespace cl
{
class CloseButtonListenerCardboard : public EventGazeListener
{
public:
  CloseButtonListenerCardboard(ILoggerFactory *loggerFactory);
  void onGazeStarted();
  void onGazeEnded();
  void onGaze();
  bool inFocus();

  bool isLongGaze();

private:
  std::unique_ptr<ILogger> logger;
  std::string tag = "CloseButtonListenerCardboard";
  bool focus = false;

  int closeCountDown = 150;
};
}

#endif //CARDBOARDIMPLEMENTATION_CLOSEBUTTONLISTENER_H