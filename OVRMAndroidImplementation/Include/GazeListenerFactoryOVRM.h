#ifndef ANDROIDIMPLEMENTATION_GAZELISTENERFACTORYOVRM_H
#define ANDROIDIMPLEMENTATION_GAZELISTENERFACTORYOVRM_H

#include <coreEngine/factory/IEventGazeListenerFactory.h>
#include <CloseMeListenerOVRM.h>
#include <NotifyMeListenerOVRM.h>

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
    std::unique_ptr<EventGazeListener> createNotifyMeListener()
    {
	return std::unique_ptr<EventGazeListener>(new NotifyMeListenerOVRM(loggerFactory));
    }

    std::unique_ptr<EventGazeListener> createCloseMeListener()
    {
	return std::unique_ptr<EventGazeListener>(new CloseMeListenerOVRM(loggerFactory));
    }

  private:
    ILoggerFactory *loggerFactory = nullptr;
};
}

#endif //ANDROIDIMPLEMENTATION_GAZELISTENERFACTORYOVRM_H