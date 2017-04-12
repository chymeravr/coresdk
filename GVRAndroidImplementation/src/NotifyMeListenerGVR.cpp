#include <NotifyMeListenerGVR.h>
#include <assert.h>

// a listener on the close button / box in the scene - we may want to animate this in future
namespace cl
{
NotifyMeListenerGVR::NotifyMeListenerGVR(ILoggerFactory *loggerFactory)
{
    assert(loggerFactory != nullptr);
    this->logger = loggerFactory->createLogger(tag);
}
void NotifyMeListenerGVR::onGazeStarted()
{
    this->logger->log(LOG_DEBUG, "onGazeStarted");
    this->focus = true;
}
void NotifyMeListenerGVR::onGazeEnded()
{
    this->logger->log(LOG_DEBUG, "onGazeEnded");
    this->focus = false;
}
void NotifyMeListenerGVR::onGaze() {}

bool NotifyMeListenerGVR::inFocus()
{
    return this->focus;
}
}