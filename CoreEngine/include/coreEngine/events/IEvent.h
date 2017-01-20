#ifndef COREENGINE_IEVENT_H
#define COREENGINE_IEVENT_H

namespace cl{
	class IEvent{
	public:
		virtual ~IEvent(){}
		virtual void callListener() = 0;
	};
}

#endif //COREENGINE_IEVENT_H