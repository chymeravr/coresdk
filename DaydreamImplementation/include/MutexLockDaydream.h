//
// Created by robin_chimera on 1/2/2017.
//

#ifndef DAYDREAMIMPLEMENTATION_MUTEXLOCK_H
#define DAYDREAMIMPLEMENTATION_MUTEXLOCK_H

#include <mutex>
#include <coreEngine/util/IMutexLock.h>

namespace cl
{
class MutexLockDaydream : public IMutexLock
{
  public:
    MutexLockDaydream() {}
    ~MutexLockDaydream() {}

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

#endif //DAYDREAMIMPLEMENTATION_MUTEXLOCK_H