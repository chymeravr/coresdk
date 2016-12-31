#ifndef COREENGINE_EVENTQUEUE_H
#define COREENGINE_EVENTQUEUE_H

#include <queue>
#include <coreEngine/events/IEventQueue.h>
#include <coreEngine/util/IMutexLock.h>

namespace cl{
	class EventQueue : public IEventQueue{
	public:
		EventQueue(std::unique_ptr<IMutexLock> mutex);
		void push(std::unique_ptr<IEvent> event);
		bool empty();
		std::unique_ptr<IEvent> pop();

	protected:
		std::queue< std::unique_ptr<IEvent> > eventQueue;
		std::unique_ptr<IMutexLock> mutex;
	};
}

#endif //COREENGINE_EVENTQUEUE_H