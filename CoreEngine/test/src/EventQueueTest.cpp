#include <gtest/gtest.h>
#include <coreEngine/events/IEventQueue.h>
#include <coreEngine/events/EventQueueFactory.h>
#include <coreEngineTest/mocks/EventMock.h>
#include <coreEngineTest/mocks/MutexLockMock.h>

using ::testing::Mock;

namespace cl{
    class EventQueueTest : public ::testing::Test{
    protected:
        void SetUp(){
            EventQueueFactory eventQueueFactory;
            std::unique_ptr<IMutexLock> mutexUptr = std::unique_ptr<IMutexLock>(new MutexLockMock);
            mutex = mutexUptr.get();
            eventQueue = eventQueueFactory.create(std::move(mutexUptr));
        }
        void TearDown(){

        }
        std::unique_ptr<IEventQueue> eventQueue;
        IMutexLock *mutex;
    };

    TEST_F(EventQueueTest, TestEmptyQueue){
        MutexLockMock &mutexMock = *(MutexLockMock*)mutex;
        EXPECT_CALL(mutexMock, lock()).Times(1);
        EXPECT_CALL(mutexMock, unlock()).Times(1);
        EXPECT_TRUE(eventQueue->empty());
    }

    TEST_F(EventQueueTest, TestPush){
        MutexLockMock &mutexMock = *(MutexLockMock*)mutex;
        
        std::unique_ptr<IEvent> event1(new EventMock);

        EXPECT_CALL(mutexMock, lock()).Times(1);
        EXPECT_CALL(mutexMock, unlock()).Times(1);
        eventQueue->push(std::move(event1));

        EXPECT_CALL(mutexMock, lock()).Times(1);
        EXPECT_CALL(mutexMock, unlock()).Times(1);
        EXPECT_FALSE(eventQueue->empty());
        
        //event = eventQueue->pop();
        //EXPECT_NE(nullptr, event);
        //EventMock eventMock = *(EventMock*)event.get();
        //EXPECT_CALL(eventMock, callListener());
        //EXPECT_TRUE(eventQueue->empty());
    }

    TEST_F(EventQueueTest, TestPop){
        MutexLockMock &mutexMock = *(MutexLockMock*)mutex;

        std::unique_ptr<IEvent> event1(new EventMock);
        std::unique_ptr<IEvent> event2(new EventMock);
        
        eventQueue->push(std::move(event1));
        eventQueue->push(std::move(event2));
        
        EXPECT_CALL(mutexMock, lock()).Times(1);
        EXPECT_CALL(mutexMock, unlock()).Times(1);
        event1 = eventQueue->pop();
        EventMock &eventMock1 = *(EventMock*)event1.get();
        EXPECT_CALL(eventMock1, callListener()).Times(1);
        eventMock1.callListener();

        Mock::VerifyAndClearExpectations(&mutexMock);

        EXPECT_FALSE(eventQueue->empty());
        
        event2 = eventQueue->pop();
        EventMock &eventMock2 = *(EventMock*)event2.get();
        EXPECT_CALL(eventMock2, callListener()).Times(1);
        eventMock2.callListener();
        EXPECT_TRUE(eventQueue->empty());
    }
}