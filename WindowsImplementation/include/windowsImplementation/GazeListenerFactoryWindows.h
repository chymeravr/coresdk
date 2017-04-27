#ifndef WINDOWSIMPLEMENTATION_GAZELISTENERFACTORYWINDOWS_H
#define WINDOWSIMPLEMENTATION_GAZELISTENERFACTORYWINDOWS_H

#include <coreEngine/factory/IEventGazeListenerFactory.h>
#include <windowsImplementation/CloseButtonListenerWindows.h>
#include <windowsImplementation/ActionButtonListenerWindows.h>

namespace cl
{
class GazeListenerFactoryWindows : public IEventGazeListenerFactory
{
  public:
    GazeListenerFactoryWindows(ILoggerFactory *loggerFactory)
    {
	assert(loggerFactory != nullptr);
	this->loggerFactory = loggerFactory;
    }
    std::unique_ptr<EventGazeListener> createCloseButtonListener()
    {
	return std::unique_ptr<EventGazeListener>(new CloseButtonListenerWindows(loggerFactory));
    }

    std::unique_ptr<EventGazeListener> createActionButtonListener()
    {
	return std::unique_ptr<EventGazeListener>(new ActionButtonListenerWindows(loggerFactory));
    }

  private:
    ILoggerFactory *loggerFactory = nullptr;
};
}

#endif //WINDOWSIMPLEMENTATION_GAZELISTENERFACTORYWINDOWS_H