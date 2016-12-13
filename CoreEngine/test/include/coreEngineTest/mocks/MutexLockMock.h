#ifndef COREENGINETEST_MUTEXLOCKMOCK_H
#define COREENGINETEST_MUTEXLOCKMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/util/IMutexLock.h>

namespace cl{
    class MutexLockMock : public IMutexLock{
    public:
        MOCK_METHOD0(lock, void(void));
        MOCK_METHOD0(unlock, void(void));
    };
}

#endif //COREENGINETEST_MUTEXLOCKMOCK_H