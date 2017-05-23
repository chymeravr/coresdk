//
// Created by robin_chimera on 1/2/2017.
//

#ifndef CARDBOARDIMPLEMENTATION_MUTEXLOCK_H
#define CARDBOARDIMPLEMENTATION_MUTEXLOCK_H

#include <mutex>
#include <coreEngine/util/IMutexLock.h>

namespace cl
{
class MutexLockCardboard : public IMutexLock
{
  public:
    MutexLockCardboard() {}
    ~MutexLockCardboard() {}

    /* TODO: figure out if the use of the below locking and unlocking mechanism will
         * cause deadlock if the application has an exception in between the two functions
         */

    void lock()
    {
        mutexLock.lock();
    }
    void unlock()
    {
        mutexLock.unlock();
    }

  private:
    std::mutex mutexLock;
};
}

#endif //CARDBOARDIMPLEMENTATION_MUTEXLOCK_H
