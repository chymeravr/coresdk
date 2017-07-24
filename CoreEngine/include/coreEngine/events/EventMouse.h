#ifndef COREENGINE_EVENTMOUSE_H
#define COREENGINE_EVENTMOUSE_H

#include <coreEngine/events/EventMouseListener.h>
#include <coreEngine/events/IEvent.h>

namespace cl {
enum MOUSE_BUTTON_STATE { MOUSE_BUTTON_UP, MOUSE_BUTTON_DOWN };
enum MOUSE_BUTTON { LEFT_BUTTON, MIDDLE_BUTTON, RIGHT_BUTTON };
class EventMouse : public IEvent {
 public:
  EventMouse(EventMouseListener *listener, MOUSE_BUTTON button,
             MOUSE_BUTTON_STATE state) {
    this->listener = listener;
    this->button = button;
    this->state = state;
  }
  void callListener() {
    switch (button) {
      case LEFT_BUTTON:
        if (state == MOUSE_BUTTON_UP) {
          listener->onLeftMouseUp();
        } else if (state == MOUSE_BUTTON_DOWN) {
          listener->onLeftMouseDown();
        }
        break;
      case MIDDLE_BUTTON:
        if (state == MOUSE_BUTTON_UP) {
          listener->onMiddleMouseUp();
        } else if (state == MOUSE_BUTTON_DOWN) {
          listener->onMiddleMouseDown();
        }
        break;
      case RIGHT_BUTTON:
        if (state == MOUSE_BUTTON_UP) {
          listener->onRightMouseUp();
        } else if (state == MOUSE_BUTTON_DOWN) {
          listener->onRightMouseDown();
        }
        break;
    }
  }

 private:
  EventMouseListener *listener;
  MOUSE_BUTTON button;
  MOUSE_BUTTON_STATE state;
};
}

#endif  // COREENGINE_EVENTMOUSE_H