#ifndef ANDROIDIMPLEMENTATION_GAZELISTENERFACTORYGVR_H
#define ANDROIDIMPLEMENTATION_GAZELISTENERFACTORYGVR_H

#include <coreEngine/factory/IEventGazeListenerFactory.h>
#include <CloseMeListenerGVR.h>
#include <NotifyMeListenerGVR.h>

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
    std::unique_ptr<EventGazeListener> createNotifyMeListener()
    {
        return std::unique_ptr<EventGazeListener>(new NotifyMeListenerGVR(loggerFactory));
    }

    std::unique_ptr<EventGazeListener> createCloseMeListener()
    {
        return std::unique_ptr<EventGazeListener>(new CloseMeListenerGVR(loggerFactory));
    }

  private:
    ILoggerFactory *loggerFactory = nullptr;
};
}

#endif //ANDROIDIMPLEMENTATION_GAZELISTENERFACTORYGVR_H