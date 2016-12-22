#ifndef COREENGINE_EVENTMOUSELISTENER_H
#define COREENGINE_EVENTMOUSELISTENER_H

namespace cl{
    class EventMouseListener{
    public:
        virtual void onLeftMouseUp(int x, int y){}
        virtual void onMiddleMouseUp(int x, int y){}
        virtual void onRightMouseUp(int x, int y){}
        virtual void onLeftMouseDown(int x, int y){}
        virtual void onMiddleMouseDown(int x, int y){}
        virtual void onRightMouseDown(int x, int y){}
    };
}

#endif //COREENGINE_EVENTMOUSELISTENER_H