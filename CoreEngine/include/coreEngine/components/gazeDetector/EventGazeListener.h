#ifndef COREENGINE_EVENTGAZELISTENER_H
#define COREENGINE_EVENTGAZELISTENER_H

namespace cl{
	class EventGazeListener{
	public:
		virtual void onGazeStarted(){}
		virtual void onGazeEnded(){}
		virtual void onGaze(){}
	};
}

#endif //COREENGINE_EVENTGAZELISTENER_H