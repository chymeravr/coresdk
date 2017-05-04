#include <CloseButtonListenerOVRM.h>
#include <assert.h>

// a listener on the close button / box in the scene - we may want to animate this in future
namespace cl
{
CloseButtonListenerOVRM::CloseButtonListenerOVRM(ILoggerFactory *loggerFactory)
{
    assert(loggerFactory != nullptr);
    this->logger = loggerFactory->createLogger(tag);
}
void CloseButtonListenerOVRM::onGazeStarted()
{
    this->focus = true;
}
void CloseButtonListenerOVRM::onGazeEnded()
{
    this->focus = false;
}
void CloseButtonListenerOVRM::onGaze() {}

bool CloseButtonListenerOVRM::inFocus()
{
    return this->focus;
}
}