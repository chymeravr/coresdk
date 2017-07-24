#ifndef COREENGINE_EVENTCONTROLLERQUATERNION_H
#define COREENGINE_EVENTCONTROLLERQUATERNION_H

#include <coreEngine/events/EventControllerQuaternionListener.h>
#include <coreEngine/events/IEvent.h>

namespace cl {
class EventControllerQuaternion : public IEvent {
 public:
  EventControllerQuaternion(EventControllerQuaternionListener *listener,
                            float qx, float qy, float qz, float qw) {
    this->listener = listener;
    this->qx = qx;
    this->qy = qy;
    this->qz = qz;
    this->qw = qw;
  }
  void callListener() {
    listener->updateControllerQuaternion(this->qx, this->qy, this->qz,
                                         this->qw);
  }

 private:
  EventControllerQuaternionListener *listener;
  float qx;
  float qy;
  float qz;
  float qw;
};
}

#endif  // COREENGINE_EVENTCONTROLLERQUATERNION_H