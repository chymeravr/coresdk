#ifndef COREENGINE_EVENTPASSIVEMOUSEMOTIONLISTENER_H
#define COREENGINE_EVENTPASSIVEMOUSEMOTIONLISTENER_H

namespace cl{
	class EventPassiveMouseMotionListener{
	public:
		virtual void onPassiveMouseMotion(int x, int y){}
	};
}

#endif //COREENGINE_EVENTPASSIVEMOUSEMOTIONLISTENER_H