#include <CloseMeListenerGVR.h>
#include <assert.h>

// a listener on the close button / box in the scene - we may want to animate this in future
namespace cl
{
CloseMeListenerGVR::CloseMeListenerGVR(ILoggerFactory *loggerFactory)
{
    assert(loggerFactory != nullptr);
    this->logger = loggerFactory->createLogger(tag);
}
void CloseMeListenerGVR::onGazeStarted()
{
    this->logger->log(LOG_DEBUG, "onGazeStarted");
    this->focus = true;
}
void CloseMeListenerGVR::onGazeEnded()
{
    this->logger->log(LOG_DEBUG, "onGazeEnded");
    this->focus = false;
}
void CloseMeListenerGVR::onGaze() {}

bool CloseMeListenerGVR::inFocus()
{
    return this->focus;
}
}