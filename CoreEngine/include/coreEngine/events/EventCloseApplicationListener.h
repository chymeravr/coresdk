#ifndef COREENGINE_EVENTCLOSEAPPLICATIONLISTENER_H
#define COREENGINE_EVENTCLOSEAPPLICATIONLISTENER_H

namespace cl {
class EventCloseApplicationListener {
 public:
  virtual void closeApplication() = 0;
};
}

#endif  // COREENGINE_EVENTCLOSEAPPLICATIONLISTENER_H