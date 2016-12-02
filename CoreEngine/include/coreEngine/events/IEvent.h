#ifndef COREENGINE_IEVENT_H
#define COREENGINE_IEVENT_H

#include <coreEngine/events/EventArgs.h>
#include <coreEngine/events/IEventListener.h>

namespace cl{
    class IEvent{
        virtual void callListener() = 0;
    };
}

#endif //COREENGINE_IEVENT_H