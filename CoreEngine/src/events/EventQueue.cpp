#include <coreEngine/events/EventQueue.h>

namespace cl{
    EventQueue::EventQueue(std::unique_ptr<IMutexLock> mutex){
        this->mutex = std::move(mutex);
    }

    void EventQueue::push(std::unique_ptr<IEvent> event){
        mutex->lock();
        eventQueue.push(std::move(event));
        mutex->unlock();
    }
    
    bool EventQueue::empty(){
        bool retVal;
        mutex->lock();
        retVal = this->eventQueue.empty();
        mutex->unlock();
        return retVal;

    }

    std::unique_ptr<IEvent> EventQueue::pop(){
        mutex->lock();
        std::unique_ptr<IEvent> event = std::move(eventQueue.front());
        this->eventQueue.pop();
        mutex->unlock();
        return event;
    }

}