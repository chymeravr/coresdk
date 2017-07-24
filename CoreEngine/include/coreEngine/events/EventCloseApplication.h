#ifndef COREENGINE_EVENTCLOSEAPPLICATION_H
#define COREENGINE_EVENTCLOSEAPPLICATION_H

#include <coreEngine/events/EventCloseApplicationListener.h>
#include <coreEngine/events/IEvent.h>

namespace cl {
class EventCloseApplication : public IEvent {
 public:
  EventCloseApplication(EventCloseApplicationListener *listener) {
    this->listener = listener;
  }
  void callListener() { listener->closeApplication(); }

 private:
  EventCloseApplicationListener *listener;
};
}

#endif  // COREENGINE_EVENTCLOSEAPPLICATION_H