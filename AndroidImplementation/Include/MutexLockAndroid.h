//
// Created by robin_chimera on 1/2/2017.
//

#ifndef GRAPHICSSDK_MUTEXLOCKANDROID_H
#define GRAPHICSSDK_MUTEXLOCKANDROID_H

#include <mutex>
#include <coreEngine/util/IMutexLock.h>

namespace cl{
    class MutexLockAndroid : public IMutexLock{
    public:
        MutexLockAndroid(){ }
        ~MutexLockAndroid(){ }

        /* TODO: figure out if the use of the below locking and unlocking mechanism will
         * cause deadlock if the application has an exception in between the two functions
         */

        void lock(){
            mutexLock.lock();
        }
        void unlock(){
            mutexLock.unlock();
        }
    private:
        std::mutex mutexLock;
    };
}

#endif //GRAPHICSSDK_MUTEXLOCKANDROID_H
