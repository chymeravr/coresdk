#ifndef COREENGINE_APPEVENT_H
#define COREENGINE_APPEVENT_H

#include <memory>
#include <coreEngine/events/IEvent.h>
#include <coreEngine/events/AppEventListener.h>

namespace cl{
	enum APP_EVENT_TYPE{
		APP_EVENT_START,
		APP_EVENT_PAUSE,
		APP_EVENT_RESUME,
		APP_EVENT_STOP
	};
	class EventStart : public IEvent{
	public:
		EventStart(AppEventListener *eventListener, APP_EVENT_TYPE eventType);
		void callListener(){}

	private:
		APP_EVENT_TYPE eventType;
		AppEventListener *eventListener;
	};
}

#endif //COREENGINE_EVENTSTART_H