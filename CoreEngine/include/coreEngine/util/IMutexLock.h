#ifndef COREENGINE_MUTEXLOCK_H
#define COREENGINE_MUTEXLOCK_H

namespace cl{
	class IMutexLock{
	public:
		virtual ~IMutexLock(){}
		virtual void lock() = 0;
		virtual void unlock() = 0;
	};
}

#endif //COREENGINE_MUTEXLOCK_H