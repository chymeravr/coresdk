//
// Created by chimeralabs on 9/23/2016.
//

#ifndef ANDROIDSDK_EVENT_H
#define ANDROIDSDK_EVENT_H

#include <coreEngine/util/EventArgs.h>

namespace cl {
    class Event {
    public:enum EVENT_TYPE {
            START,
            STOP,
            PAUSE,
            PLAY,
            ON_KEY_DOWN,
            ON_TOUCH
        };
    private:
        EVENT_TYPE eventType;
        EventArgs *eventArgs;
    public:
        Event(EVENT_TYPE evenType, EventArgs *eventArgs);
        Event::EVENT_TYPE getEventType();
        EventArgs *getEventArgs();
    };
}
#endif //ANDROIDSDK_EVENT_H
