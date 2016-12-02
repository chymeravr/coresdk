//
// Created by chimeralabs on 9/23/2016.
//
/**
 *
 * EventQueue: Queue for events. It is a singleton class. It is a threadsafe class.
 *
 */
#ifndef ANDROIDSDK_EVENTQUEUE_H
#define ANDROIDSDK_EVENTQUEUE_H

#include <queue>
#include <memory>
#include <coreEngine/util/IMutexLock.h>
#include <coreEngine/events/IEvent.h>

namespace cl{
    /*
    * Thread save event queue
    */
    class EventQueue{
    public:
        EventQueue(std::unique_ptr<IMutexLock> mutex);
        void push(std::unique_ptr<IEvent> event);
        bool empty();
        std::unique_ptr<IEvent> pop();

    private:
        std::queue< std::unique_ptr<IEvent> > eventQueue;
        std::unique_ptr<IMutexLock> mutex;
    };

}
#endif //ANDROIDSDK_EVENTQUEUE_H
