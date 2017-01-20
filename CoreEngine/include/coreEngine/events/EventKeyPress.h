#ifndef COREENGINE_EVENTKEYPRESS_H
#define COREENGINE_EVENTKEYPRESS_H

#include <memory>
#include <coreEngine/events/IEvent.h>
#include <coreEngine/events/EventKeyPressListener.h>

namespace cl{
	class EventKeyPress : public IEvent{
	public:
		EventKeyPress(EventKeyPressListener *listener, char key, int x, int y){
			this->listener = listener;
			this->key = key;
			this->x = x;
			this->y = y;
		}
		void callListener(){
			listener->onKeyPress(key, x, y);
		}
	private:
		EventKeyPressListener* listener;
		char key;
		int x;
		int y;
	};
}

#endif //COREENGINE_EVENTKEYPRESS_H