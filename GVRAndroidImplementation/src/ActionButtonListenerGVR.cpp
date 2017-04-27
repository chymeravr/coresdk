#include <ActionButtonListenerGVR.h>
#include <assert.h>

// a listener on the close button / box in the scene - we may want to animate this in future
namespace cl
{
ActionButtonListenerGVR::ActionButtonListenerGVR(ILoggerFactory *loggerFactory)
{
    assert(loggerFactory != nullptr);
    this->logger = loggerFactory->createLogger(tag);
}
void ActionButtonListenerGVR::onGazeStarted()
{
    this->focus = true;
}
void ActionButtonListenerGVR::onGazeEnded()
{
    this->focus = false;
}
void ActionButtonListenerGVR::onGaze() {}

bool ActionButtonListenerGVR::inFocus()
{
    return this->focus;
}
}