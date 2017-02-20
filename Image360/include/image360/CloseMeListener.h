#ifndef IMAGE360_CLOSEMELISTENER_H
#define IMAGE360_CLOSEMELISTENER_H

#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <iostream>

namespace cl{
	class CloseMeListener : public EventGazeListener{
	public:
		void onGazeStarted(){
			std::cout << "CloseMe: OnGazeStarted" << std::endl;
		}
		void onGazeEnded(){
			std::cout << "CloseMe: OnGazeEnded" << std::endl;
		}
		void onGaze(){
			std::cout << "CloseMe: OnGaze" << std::endl;
		}
	};
}

#endif //IMAGE360_CLOSEMELISTENER_H