#include <ActionButtonListenerCardboard.h>
#include <assert.h>

// a listener on the close button / box in the scene - we may want to animate this in future
namespace cl
{
ActionButtonListenerCardboard::ActionButtonListenerCardboard(ILoggerFactory *loggerFactory)
{
    assert(loggerFactory != nullptr);
    this->logger = loggerFactory->createLogger(tag);
}
void ActionButtonListenerCardboard::onGazeStarted()
{
    this->focus = true;
}
void ActionButtonListenerCardboard::onGazeEnded()
{
    this->focus = false;
}
void ActionButtonListenerCardboard::onGaze() {}

bool ActionButtonListenerCardboard::inFocus()
{
    return this->focus;
}
}