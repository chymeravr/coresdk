#ifndef WINDOWSIMPLEMENTATION_NOTIFYMELISTENER_H
#define WINDOWSIMPLEMENTATION_NOTIFYMELISTENER_H

#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <assert.h>

namespace cl
{
class ActionButtonListenerWindows : public EventGazeListener
{
  public:
    ActionButtonListenerWindows(ILoggerFactory *loggerFactory)
    {
	assert(loggerFactory != nullptr);
	this->logger = loggerFactory->createLogger(tag);
    }
    void onGazeStarted()
    {
	logger->log(LOG_INFO, "Action Button: OnGazeStarted");
	this->focus = true;
    }
    void onGazeEnded()
    {
	logger->log(LOG_INFO, "Action Button: OnGazeEnded");
	this->focus = false;
    }
    void onGaze()
    {
	//logger->log(LOG_INFO, "NotifyMe: OnGaze");
    }
    bool inFocus()
    {
	return this->focus;
    }

  private:
    std::unique_ptr<ILogger> logger;
    std::string tag = "ActionButtonListenerWindows";
    bool focus = false;
};
}

#endif //WINDOWSIMPLEMENTATION_NOTIFYMELISTENER_H