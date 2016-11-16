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
#include <pthread.h>
#include <coreEngine/util/Event.h>

namespace cl{
    class EventQueue{
    private:
        std::queue<Event> eventQueue;
        pthread_mutex_t mutex;
        static EventQueue *instance;

    public:
        EventQueue();
        ~EventQueue();
        void push(Event event);
        bool empty();
        Event pop();
        static EventQueue* getInstance();
    };

}
#endif //ANDROIDSDK_EVENTQUEUE_H
