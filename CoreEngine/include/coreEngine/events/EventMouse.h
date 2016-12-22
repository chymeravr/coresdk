#ifndef COREENGINE_EVENTMOUSE_H
#define COREENGINE_EVENTMOUSE_H

#include <coreEngine/events/IEvent.h>
#include <coreEngine/events/EventMouseListener.h>

namespace cl{
    enum MOUSE_BUTTON_STATE{
        MOUSE_BUTTON_UP,
        MOUSE_BUTTON_DOWN
    }
    enum MOUSE_BUTTON{
        LEFT_BUTTON,
        MIDDLE_BUTTON,
        RIGHT_BUTTON
    };
    class EventMouse : public IEvent{
    public:
        EventMouse(EventMouseListener *listener, MOUSE_BUTTON button, MOUSE_BUTTON_STATE state){
            this->listener = listener;
            this->button = button;
            this->state = state;
        }
        void callListener(){
            switch (button){
            case LEFT_BUTTON:
                if (state == MOUSE_BUTTON_UP){
                    listener->onLeftMouseUp(x, y);
                }
                else if (state == MOUSE_BUTTON_DOWN){
                    listener->onLeftMouseDown(x, y);
                }
                break;
            case MIDDLE_BUTTON:
                if (state == MOUSE_BUTTON_UP){
                    listener->onMiddleMouseUp(x, y);
                }
                else if (state == MOUSE_BUTTON_DOWN){
                    listener->onMiddleMouseDown(x, y);
                }
                break;
            case RIGHT_BUTTON:
                if (state == MOUSE_BUTTON_UP){
                    listener->onRightMouseUp(x, y);
                }
                else if (state == MOUSE_BUTTON_DOWN){
                    listener->onRightMouseDown(x, y);
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

#endif //COREENGINE_EVENTMOUSE_H