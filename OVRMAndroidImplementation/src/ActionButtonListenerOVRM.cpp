#include <ActionButtonListenerOVRM.h>
#include <assert.h>

// a listener on the close button / box in the scene - we may want to animate this in future
namespace cl
{
ActionButtonListenerOVRM::ActionButtonListenerOVRM(ILoggerFactory *loggerFactory)
{
    assert(loggerFactory != nullptr);
    this->logger = loggerFactory->createLogger(tag);
}
void ActionButtonListenerOVRM::onGazeStarted()
{
    this->focus = true;
}
void ActionButtonListenerOVRM::onGazeEnded()
{
    this->focus = false;
}
void ActionButtonListenerOVRM::onGaze() {}

bool ActionButtonListenerOVRM::inFocus()
{
    return this->focus;
}
}