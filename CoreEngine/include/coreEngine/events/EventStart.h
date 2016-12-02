#ifndef COREENGINE_EVENTSTART_H
#define COREENGINE_EVENTSTART_H

#include <memory>
#include <coreEngine/events/IEvent.h>
#include <coreEngine/events/EventStartListener.h>

namespace cl{
    class EventStart : public IEvent{
    public:
        EventStart(EventStartListener *eventListener, const EventArgs &eventArgs){
            this->eventListener = eventListener;
            this->eventArgs = eventArgs;
        }
        void callListener(){
            eventListener->onEventStart(eventArgs);
        }

    private:
        EventArgs eventArgs;
        EventStartListener *eventListener;
    };
}

#endif //COREENGINE_EVENTSTART_H