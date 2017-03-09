#ifndef COREENGINE_EVENTGAZELISTENER_H
#define COREENGINE_EVENTGAZELISTENER_H

namespace cl{
	class EventGazeListener{
	public:
		virtual void onGazeStarted() = 0;
		virtual void onGazeEnded() = 0;
		virtual void onGaze() = 0;
		virtual bool inFocus() = 0;
	};
}

#endif //COREENGINE_EVENTGAZELISTENER_H