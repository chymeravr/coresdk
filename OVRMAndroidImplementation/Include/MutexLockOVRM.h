//
// Created by robin_chimera on 1/2/2017.
//

#ifndef ANDROIDIMPLEMENTATION_MUTEXLOCKOVRM_H
#define ANDROIDIMPLEMENTATION_MUTEXLOCKOVRM_H

#include <mutex>
#include <coreEngine/util/IMutexLock.h>

namespace cl
{
class MutexLockOVRM : public IMutexLock
{
  public:
    MutexLockOVRM() {}
    ~MutexLockOVRM() {}

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

#endif //ANDROIDIMPLEMENTATION_MUTEXLOCKOVRM_H
