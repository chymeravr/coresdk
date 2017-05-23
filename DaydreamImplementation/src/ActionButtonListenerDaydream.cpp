#include <ActionButtonListenerDaydream.h>
#include <assert.h>

// a listener on the close button / box in the scene - we may want to animate this in future
namespace cl
{
ActionButtonListenerDaydream::ActionButtonListenerDaydream(ILoggerFactory *loggerFactory)
{
    assert(loggerFactory != nullptr);
    this->logger = loggerFactory->createLogger(tag);
}
void ActionButtonListenerDaydream::onGazeStarted()
{
    this->focus = true;
}
void ActionButtonListenerDaydream::onGazeEnded()
{
    this->focus = false;
}
void ActionButtonListenerDaydream::onGaze() {}

bool ActionButtonListenerDaydream::inFocus()
{
    return this->focus;
}
}