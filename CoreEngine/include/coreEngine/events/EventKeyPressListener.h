#ifndef COREENGINE_EVENTKEYPRESSLISTENER_H
#define COREENGINE_EVENTKEYPRESSLISTENER_H

namespace cl{
	class EventKeyPressListener{
	public:
		/*
		* Called when key is pressed. x and y are coordinates of mouse position when key is pressed. When mouse position can't be captured
		* when key is pressed, negative coordinates will be returned.
		*/
		virtual void onKeyPress(char key, int x, int y) = 0;
	};
}

#endif //COREENGINE_EVENTKEYPRESSLISTENER_H