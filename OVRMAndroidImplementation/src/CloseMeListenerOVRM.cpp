#include <CloseMeListenerOVRM.h>
#include <assert.h>

// a listener on the close button / box in the scene - we may want to animate this in future
namespace cl
{
CloseMeListenerOVRM::CloseMeListenerOVRM(ILoggerFactory *loggerFactory)
{
    assert(loggerFactory != nullptr);
    this->logger = loggerFactory->createLogger(tag);
}
void CloseMeListenerOVRM::onGazeStarted()
{
    this->focus = true;
}
void CloseMeListenerOVRM::onGazeEnded()
{
    this->focus = false;
}
void CloseMeListenerOVRM::onGaze() {}

bool CloseMeListenerOVRM::inFocus()
{
    return this->focus;
}
}