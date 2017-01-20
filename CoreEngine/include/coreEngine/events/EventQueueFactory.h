#ifndef COREENGINE_EVENTQUEUEFACTORY_H
#define COREENGINE_EVENTQUEUEFACTORY_H

#include <memory>
#include <coreEngine/events/EventQueue.h>

namespace cl{
	class EventQueueFactory{
	public:
		std::unique_ptr<IEventQueue> create(std::unique_ptr<IMutexLock> mutex){
			return std::unique_ptr<IEventQueue>(new EventQueue(std::move(mutex)));
		}
	};
}

#endif //COREENGINE_EVENTQUEUEFACTORY_H