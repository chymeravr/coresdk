#ifndef COREENGINE_EVENTSTARTLISTENER_H
#define COREENGINE_EVENTSTARTLISTENER_H

namespace cl{
    class EventStartListener{
    public:
        virtual void onEventStart(const EventArgs &eventArgs) = 0;
    };
}

#endif //COREENGINE_EVENTSTARTLISTENER_H