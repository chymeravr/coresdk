#ifndef DAYDREAMIMPLEMENTATION_GAZELISTENERFACTORY_H
#define DAYDREAMIMPLEMENTATION_GAZELISTENERFACTORY_H

#include <coreEngine/factory/IEventGazeListenerFactory.h>
#include <CloseButtonListenerDaydream.h>
#include <ActionButtonListenerDaydream.h>

namespace cl
{
class GazeListenerFactoryDaydream : public IEventGazeListenerFactory
{
  public:
    GazeListenerFactoryDaydream(ILoggerFactory *loggerFactory)
    {
        assert(loggerFactory != nullptr);
        this->loggerFactory = loggerFactory;
    }
    std::unique_ptr<EventGazeListener> createActionButtonListener()
    {
        return std::unique_ptr<EventGazeListener>(new ActionButtonListenerDaydream(loggerFactory));
    }

    std::unique_ptr<EventGazeListener> createCloseButtonListener()
    {
        return std::unique_ptr<EventGazeListener>(new CloseButtonListenerDaydream(loggerFactory));
    }

  private:
    ILoggerFactory *loggerFactory = nullptr;
};
}

#endif //DAYDREAMIMPLEMENTATION_GAZELISTENERFACTORY_H