#ifndef WINDOWSIMPLEMENTATION_GAZELISTENERFACTORYWINDOWS_H
#define WINDOWSIMPLEMENTATION_GAZELISTENERFACTORYWINDOWS_H

#include <coreEngine/factory/IEventGazeListenerFactory.h>
#include <windowsImplementation/CloseMeListenerWindows.h>
#include <windowsImplementation/NotifyMeListenerWindows.h>

namespace cl{
	class GazeListenerFactoryWindows : public IEventGazeListenerFactory{
	public:
		GazeListenerFactoryWindows(ILoggerFactory *loggerFactory){
			assert(loggerFactory != nullptr);
			this->loggerFactory = loggerFactory;
		}
		std::unique_ptr<EventGazeListener> createNotifyMeListener(){
			return std::unique_ptr<EventGazeListener>(new CloseMeListenerWindows(loggerFactory)); 
		}

		std::unique_ptr<EventGazeListener> createCloseMeListener(){
			return std::unique_ptr<EventGazeListener>(new NotifyMeListenerWindows(loggerFactory));
		}
	private:
		ILoggerFactory *loggerFactory = nullptr;
	};
}

#endif //WINDOWSIMPLEMENTATION_GAZELISTENERFACTORYWINDOWS_H