//
// Created by chimeralabs on 9/23/2016.
//

#include <coreEngine/util/EventQueue.h>

namespace cl{
    EventQueue *EventQueue::instance = NULL;
    EventQueue::EventQueue() {
        pthread_mutex_init(&this->mutex, 0);
    }

    EventQueue::~EventQueue() {
        pthread_mutex_destroy(&this->mutex);
    }

    EventQueue* EventQueue::getInstance() {
        if(instance == NULL) {
            instance = new EventQueue();
        }
        return instance;
    }

    void EventQueue::push(Event event) {
        pthread_mutex_lock(&this->mutex);
        this->eventQueue.push(event);
        pthread_mutex_unlock(&this->mutex);
    }

    Event EventQueue::pop() {
        pthread_mutex_lock(&this->mutex);
        Event event = this->eventQueue.front();
        this->eventQueue.pop();
        pthread_mutex_unlock(&this->mutex);
        return event;
    }

    bool EventQueue::empty() {
        bool retVal;
        pthread_mutex_lock(&this->mutex);
        retVal = this->eventQueue.empty();
        pthread_mutex_unlock(&this->mutex);
        return retVal;
    }
}