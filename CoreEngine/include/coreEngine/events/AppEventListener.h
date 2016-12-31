#ifndef COREENGINE_APPEVENTLISTENER_H
#define COREENGINE_APPEVENTLISTENER_H

namespace cl{
	class AppEventListener{
	public:
		virtual void onAppStartEvent() = 0;
		virtual void onAppPauseEvent() = 0;
		virtual void onAppResumeEvent() = 0;
		virtual void onAppStopEvent() = 0;
	};
}

#endif //COREENGINE_APPEVENTLISTENER_H