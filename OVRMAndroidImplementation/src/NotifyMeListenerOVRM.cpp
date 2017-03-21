#include <NotifyMeListenerOVRM.h>
#include <assert.h>

// a listener on the close button / box in the scene - we may want to animate this in future
namespace cl
{
NotifyMeListenerOVRM::NotifyMeListenerOVRM(ILoggerFactory *loggerFactory)
{
    assert(loggerFactory != nullptr);
    this->logger = loggerFactory->createLogger(tag);
}
void NotifyMeListenerOVRM::onGazeStarted()
{
    this->focus = true;
}
void NotifyMeListenerOVRM::onGazeEnded()
{
    this->focus = false;
}
void NotifyMeListenerOVRM::onGaze() {}

bool NotifyMeListenerOVRM::inFocus()
{
    return this->focus;
}
}