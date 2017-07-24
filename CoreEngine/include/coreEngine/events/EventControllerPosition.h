#ifndef COREENGINE_EVENTCONTROLLERPOSITION_H
#define COREENGINE_EVENTCONTROLLERPOSITION_H

#include <coreEngine/events/EventControllerPositionListener.h>
#include <coreEngine/events/IEvent.h>

namespace cl {
class EventControllerPosition : public IEvent {
 public:
  EventControllerPosition(EventControllerPositionListener *listener,
                          float pos_x, float pos_y, float pos_z) {
    this->listener = listener;
    this->pos_x = pos_x;
    this->pos_y = pos_y;
    this->pos_z = pos_z;
  }
  void callListener() {
    listener->updateControllerPosition(pos_x, pos_y, pos_z);
  }

 private:
  EventControllerPositionListener *listener;
  float pos_x;
  float pos_y;
  float pos_z;
};
}

#endif  // COREENGINE_EVENTCONTROLLERPOSITION_H