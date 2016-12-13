#ifndef COREENGINETEST_EVENTMOCK_H
#define COREENGINETEST_EVENTMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/events/IEvent.h>

namespace cl{
    class EventMock : public IEvent{
    public:
        MOCK_METHOD0(callListener, void(void));
    };
}

#endif //COREENGINETEST_EVENTMOCK_H