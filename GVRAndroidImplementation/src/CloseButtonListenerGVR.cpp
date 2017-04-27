#include <CloseButtonListenerGVR.h>
#include <assert.h>

// a listener on the close button / box in the scene - we may want to animate this in future
namespace cl
{
CloseButtonListenerGVR::CloseButtonListenerGVR(ILoggerFactory *loggerFactory)
{
    assert(loggerFactory != nullptr);
    this->logger = loggerFactory->createLogger(tag);
}
void CloseButtonListenerGVR::onGazeStarted()
{
    this->focus = true;
}
void CloseButtonListenerGVR::onGazeEnded()
{
    this->focus = false;
}
void CloseButtonListenerGVR::onGaze() {}

bool CloseButtonListenerGVR::inFocus()
{
    return this->focus;
}
}