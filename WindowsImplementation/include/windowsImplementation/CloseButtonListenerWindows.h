#ifndef WINDOWSIMPLEMENTATION_CLOSEMELISTENER_H
#define WINDOWSIMPLEMENTATION_CLOSEMELISTENER_H

#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <assert.h>

namespace cl
{
class CloseButtonListenerWindows : public EventGazeListener
{
  public:
    CloseButtonListenerWindows(ILoggerFactory *loggerFactory)
    {
	assert(loggerFactory != nullptr);
	this->logger = loggerFactory->createLogger(tag);
    }
    void onGazeStarted()
    {
	logger->log(LOG_INFO, "CloseMe: OnGazeStarted");
	this->focus = true;
    }
    void onGazeEnded()
    {
	logger->log(LOG_INFO, "CloseMe: OnGazeEnded");
	this->focus = false;
    }
    void onGaze()
    {
	//logger->log(LOG_INFO, "CloseMe: OnGaze");
    }
    bool inFocus()
    {
	return this->focus;
    }

  private:
    std::unique_ptr<ILogger> logger;
    std::string tag = "CloseButtonListenerWindows";
    bool focus = false;
};
}

#endif //WINDOWSIMPLEMENTATION_CLOSEMELISTENER_H