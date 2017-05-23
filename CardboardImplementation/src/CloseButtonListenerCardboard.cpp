#include <CloseButtonListenerCardboard.h>
#include <assert.h>

// a listener on the close button / box in the scene - we may want to animate this in future
namespace cl
{
CloseButtonListenerCardboard::CloseButtonListenerCardboard(ILoggerFactory *loggerFactory)
{
    assert(loggerFactory != nullptr);
    this->logger = loggerFactory->createLogger(tag);
}
void CloseButtonListenerCardboard::onGazeStarted()
{
    this->focus = true;
}
void CloseButtonListenerCardboard::onGazeEnded()
{
    this->focus = false;
}
void CloseButtonListenerCardboard::onGaze() {}

bool CloseButtonListenerCardboard::inFocus()
{
    return this->focus;
}
}