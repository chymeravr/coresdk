#ifndef CARDBOARDIMPLEMENTATION_GAZELISTENERFACTORY_H
#define CARDBOARDIMPLEMENTATION_GAZELISTENERFACTORY_H

#include <coreEngine/factory/IEventGazeListenerFactory.h>
#include <CloseButtonListenerCardboard.h>
#include <ActionButtonListenerCardboard.h>

namespace cl
{
class GazeListenerFactoryCardboard : public IEventGazeListenerFactory
{
  public:
    GazeListenerFactoryCardboard(ILoggerFactory *loggerFactory)
    {
        assert(loggerFactory != nullptr);
        this->loggerFactory = loggerFactory;
    }
    std::unique_ptr<EventGazeListener> createActionButtonListener()
    {
        return std::unique_ptr<EventGazeListener>(new ActionButtonListenerCardboard(loggerFactory));
    }

    std::unique_ptr<EventGazeListener> createCloseButtonListener()
    {
        return std::unique_ptr<EventGazeListener>(new CloseButtonListenerCardboard(loggerFactory));
    }

  private:
    ILoggerFactory *loggerFactory = nullptr;
};
}

#endif //CARDBOARDIMPLEMENTATION_GAZELISTENERFACTORY_H