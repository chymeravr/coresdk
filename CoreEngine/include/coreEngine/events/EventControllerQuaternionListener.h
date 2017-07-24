#ifndef COREENGINE_EVENTCONTROLLERQUATERNIONLISTENER_H
#define COREENGINE_EVENTCONTROLLERQUATERNIONLISTENER_H

namespace cl {
class EventControllerQuaternionListener {
 public:
  virtual void updateControllerQuaternion(float qx, float qy, float qz,
                                          float qw) = 0;
};
}

#endif  // COREENGINE_EVENTCONTROLLERQUATERNIONLISTENER_H