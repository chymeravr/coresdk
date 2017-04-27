#ifndef ANDROIDIMPLEMENTATION_GAZELISTENERFACTORYGVR_H
#define ANDROIDIMPLEMENTATION_GAZELISTENERFACTORYGVR_H

#include <coreEngine/factory/IEventGazeListenerFactory.h>
#include <CloseButtonListenerGVR.h>
#include <ActionButtonListenerGVR.h>

namespace cl
{
class GazeListenerFactoryGVR : public IEventGazeListenerFactory
{
  public:
    GazeListenerFactoryGVR(ILoggerFactory *loggerFactory)
    {
        assert(loggerFactory != nullptr);
        this->loggerFactory = loggerFactory;
    }
    std::unique_ptr<EventGazeListener> createActionButtonListener()
    {
        return std::unique_ptr<EventGazeListener>(new ActionButtonListenerGVR(loggerFactory));
    }

    std::unique_ptr<EventGazeListener> createCloseButtonListener()
    {
        return std::unique_ptr<EventGazeListener>(new CloseButtonListenerGVR(loggerFactory));
    }

  private:
    ILoggerFactory *loggerFactory = nullptr;
};
}

#endif //ANDROIDIMPLEMENTATION_GAZELISTENERFACTORYGVR_H