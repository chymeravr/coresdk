#ifndef COREENGINE_EVENTPASSIVEMOUSEMOTION_H
#define COREENGINE_EVENTPASSIVEMOUSEMOTION_H

#include <coreEngine/events/IEvent.h>
#include <coreEngine/events/EventPassiveMouseMotionListener.h>

namespace cl{
	class EventPassiveMouseMotion : public IEvent{
	public:
		EventPassiveMouseMotion(EventPassiveMouseMotionListener *listener, int x, int y){
			this->listener = listener;
			this->x = x;
			this->y = y;
		}
		void callListener(){
			listener->onPassiveMouseMotion(x, y);
		}
	private:
		EventPassiveMouseMotionListener *listener;
		int x;
		int y;
	};
}

#endif //COREENGINE_EVENTPASSIVEMOUSEMOTION_H