//
// Created by chimeralabs on 9/23/2016.
//

#include <coreEngine/util/Event.h>

namespace cl{
    Event::Event(EVENT_TYPE eventType, EventArgs *eventArgs) {
        this->eventType = eventType;
        this->eventArgs = eventArgs;
    }

    Event::EVENT_TYPE Event::getEventType() {
        return this->eventType;
    }

    EventArgs* Event::getEventArgs() {
        return this->eventArgs;
    }
}