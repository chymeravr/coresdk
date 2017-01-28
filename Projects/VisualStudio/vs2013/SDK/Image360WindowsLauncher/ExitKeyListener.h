#ifndef IMAGE360WINDOWSLAUNCHER_EXITKEYLISTENER
#define IMAGE360WINDOWSLAUNCHER_EXITKEYLISTENER

#include <coreEngine\events\EventKeyPressListener.h>

namespace cl{
	class ExitKeyListener : public EventKeyPressListener {
	public:
		ExitKeyListener::ExitKeyListener();
		void onKeyPress(char key, int x, int y);
	};
}

#endif // IMAGE360WINDOWSLAUNCHER_EXITKEYLISTENER