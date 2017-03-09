#ifndef ANDROIDIMPLEMENTATION_NOTIFYMELISTENERGVR_H
#define ANDROIDIMPLEMENTATION_NOTIFYMELISTENERGVR_H

#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <assert.h>

namespace cl{
	class NotifyMeListenerAndroidGvr : public EventGazeListener{
	public:

        NotifyMeListenerAndroidGvr(ILoggerFactory* loggerFactory);
		void onGazeStarted();
		void onGazeEnded();
		void onGaze();
        bool inFocus();
	private:
		std::unique_ptr<ILogger> logger;
		std::string tag = "NotfiyMeListenerAndroidGvr";
        bool focus;
	};
}

#endif //ANDROIDIMPLEMENTATION_NOTIFYMELISTENERGVR_H