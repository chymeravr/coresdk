#ifndef COREENGINE_IEVENTGAZELISTENERFACTORY_H
#define COREENGINE_IEVENTGAZELISTENERFACTORY_H

#include <memory>
#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <coreEngine/util/ILogger.h>

namespace cl{
	class IEventGazeListenerFactory{
	public:
		virtual std::unique_ptr<EventGazeListener> createNotifyMeListener() = 0;
		virtual std::unique_ptr<EventGazeListener> createCloseMeListener() = 0;
	};
}

#endif //COREENGINE_IEVENTGAZELISTENERFACTORY_H