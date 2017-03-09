#ifndef ANDROIDIMPLEMENTATION_CLOSEMELISTENERGVR_H
#define ANDROIDIMPLEMENTATION_CLOSEMELISTENERGVR_H

#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <assert.h>

namespace cl{
	class CloseMeListenerAndroidGvr : public EventGazeListener{
	public:
		CloseMeListenerAndroidGvr(ILoggerFactory* loggerFactory);
		void onGazeStarted();
		void onGazeEnded();
		void onGaze();
        bool inFocus();
	private:
		std::unique_ptr<ILogger> logger;
		std::string tag = "CloseMeListenerAndroidGvr";
		bool focus;
	};
}

#endif //ANDROIDIMPLEMENTATION_CLOSEMELISTENERGVR_H