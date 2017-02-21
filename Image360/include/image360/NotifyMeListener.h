#ifndef IMAGE360_NOTIFYMELISTENER_H
#define IMAGE360_NOTIFYMELISTENER_H

#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <iostream>

namespace cl{
	class NotifyMeListener : public EventGazeListener{
	public:
		void onGazeStarted(){
			std::cout << "NotifyMe: OnGazeStarted" << std::endl;
		}
		void onGazeEnded(){
			std::cout << "NotifyMe: OnGazeEnded" << std::endl;
		}
		void onGaze(){
			std::cout << "NotifyMe: OnGaze" << std::endl;
		}
	};
}

#endif //IMAGE360_NOTIFYMELISTENER_H