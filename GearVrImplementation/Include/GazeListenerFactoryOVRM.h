#ifndef ANDROIDIMPLEMENTATION_GAZELISTENERFACTORYOVRM_H
#define ANDROIDIMPLEMENTATION_GAZELISTENERFACTORYOVRM_H

#include <coreEngine/factory/IEventGazeListenerFactory.h>
#include <CloseButtonListenerOVRM.h>
#include <ActionButtonListenerOVRM.h>

namespace cl
{
class GazeListenerFactoryOVRM : public IEventGazeListenerFactory
{
  public:
    GazeListenerFactoryOVRM(ILoggerFactory *loggerFactory)
    {
        assert(loggerFactory != nullptr);
        this->loggerFactory = loggerFactory;
    }
    std::unique_ptr<EventGazeListener> createActionButtonListener()
    {
        return std::unique_ptr<EventGazeListener>(new ActionButtonListenerOVRM(loggerFactory));
    }

    std::unique_ptr<EventGazeListener> createCloseButtonListener()
    {
        return std::unique_ptr<EventGazeListener>(new CloseButtonListenerOVRM(loggerFactory));
    }

  private:
    ILoggerFactory *loggerFactory = nullptr;
};
}

#endif //ANDROIDIMPLEMENTATION_GAZELISTENERFACTORYOVRM_H