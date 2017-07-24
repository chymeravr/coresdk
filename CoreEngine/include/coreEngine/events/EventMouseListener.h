#ifndef COREENGINE_EVENTMOUSELISTENER_H
#define COREENGINE_EVENTMOUSELISTENER_H

namespace cl {
class EventMouseListener {
 public:
  virtual void onLeftMouseUp() {}
  virtual void onMiddleMouseUp() {}
  virtual void onRightMouseUp() {}
  virtual void onLeftMouseDown() {}
  virtual void onMiddleMouseDown() {}
  virtual void onRightMouseDown() {}
};
}

#endif  // COREENGINE_EVENTMOUSELISTENER_H