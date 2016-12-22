#ifndef WINDOWSIMPLEMENTATION_MUTEXLOCKWINDOWS_H
#define WINDOWSIMPLEMENTATION_MUTEXLOCKWINDOWS_H

#include <Windows.h>
#include <coreEngine/util/IMutexLock.h>

namespace cl{
    class MutexLockWindows : public IMutexLock{
    public:
        MutexLockWindows(){
            ghMutex = CreateMutex(
                NULL,              // default security attributes
                FALSE,             // initially not owned
                NULL);             // unnamed mutex

            if (ghMutex == NULL) // error in creation
            {
                
            }
        }
        ~MutexLockWindows(){
            CloseHandle(ghMutex);
        }
        void lock(){
            DWORD dwWaitResult = WaitForSingleObject(
                ghMutex,    // handle to mutex
                INFINITE);  // no time-out interval
        }
        void unlock(){
            if (!ReleaseMutex(ghMutex))
            {
                // Handle error.
            }
        }
    private:
        HANDLE ghMutex;
    };
}

#endif //WINDOWSIMPLEMENTATION_MUTEXLOCKWINDOWS_H