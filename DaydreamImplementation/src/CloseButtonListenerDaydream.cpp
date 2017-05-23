#include <CloseButtonListenerDaydream.h>
#include <assert.h>

// a listener on the close button / box in the scene - we may want to animate this in future
namespace cl
{
CloseButtonListenerDaydream::CloseButtonListenerDaydream(ILoggerFactory *loggerFactory)
{
    assert(loggerFactory != nullptr);
    this->logger = loggerFactory->createLogger(tag);
}
void CloseButtonListenerDaydream::onGazeStarted()
{
    this->focus = true;
}
void CloseButtonListenerDaydream::onGazeEnded()
{
    this->focus = false;
}
void CloseButtonListenerDaydream::onGaze() {}

bool CloseButtonListenerDaydream::inFocus()
{
    return this->focus;
}
}