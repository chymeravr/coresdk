#ifndef ANDROIDIMPLEMENTATION_GAZELISTENERFACTORYGVR_H
#define ANDROIDIMPLEMENTATION_GAZELISTENERFACTORYGVR_H

#include <coreEngine/factory/IEventGazeListenerFactory.h>
#include <CloseMeListenerAndroidGvr.h>
#include <NotifyMeListenerAndroidGvr.h>

namespace cl{
	class GazeListenerFactoryAndroidGvr : public IEventGazeListenerFactory{
	public:
		GazeListenerFactoryAndroidGvr(ILoggerFactory *loggerFactory){
			assert(loggerFactory != nullptr);
			this->loggerFactory = loggerFactory;
		}
		std::unique_ptr<EventGazeListener> createNotifyMeListener(){
			return std::unique_ptr<EventGazeListener>(new NotifyMeListenerAndroidGvr(loggerFactory)); 
		}

		std::unique_ptr<EventGazeListener> createCloseMeListener(){
			return std::unique_ptr<EventGazeListener>(new CloseMeListenerAndroidGvr(loggerFactory));
		}
	private:
		ILoggerFactory *loggerFactory = nullptr;
	};
}

#endif //ANDROIDIMPLEMENTATION_GAZELISTENERFACTORYGVR_H