//
// Created by chimeralabs on 9/23/2016.
//
#ifndef ANDROIDSDK_IEVENTQUEUE_H
#define ANDROIDSDK_IEVENTQUEUE_H

#include <queue>
#include <memory>
#include <coreEngine/util/IMutexLock.h>
#include <coreEngine/events/IEvent.h>

namespace cl{
    /*
    * Thread safe event queue
    */
    class IEventQueue{
    public:
        virtual ~IEventQueue(){}
        virtual void push(std::unique_ptr<IEvent> event) = 0;
        virtual bool empty() = 0;
        virtual std::unique_ptr<IEvent> pop() = 0;
    };

}
#endif //ANDROIDSDK_IEVENTQUEUE_H
