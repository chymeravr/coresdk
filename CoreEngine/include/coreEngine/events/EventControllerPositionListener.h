#ifndef COREENGINE_EVENTCONTROLLERPOSITIONLISTENER_H
#define COREENGINE_EVENTCONTROLLERPOSITIONLISTENER_H

namespace cl {
class EventControllerPositionListener {
 public:
  virtual void updateControllerPosition(float pos_x, float pos_y,
                                        float pos_z) = 0;
};
}

#endif  // COREENGINE_EVENTCONTROLLERPOSITIONLISTENER_H