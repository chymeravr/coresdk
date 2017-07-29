//
// Created by robin_chimera on 7/27/2017.
//

#ifndef DAYDREAMIMPLEMENTATION_CONTROLLERCLICKEVENT_H
#define DAYDREAMIMPLEMENTATION_CONTROLLERCLICKEVENT_H

#include <coreEngine/events/IEvent.h>
#include <image360/Buttons.h>

namespace cl {

class ControllerClickEventDaydream : public IEvent {
 public:
  ControllerClickEventDaydream(Buttons& buttons) { this->buttons = &buttons; }
  void callListener() { this->buttons->onClickHandler(); }

 private:
  Buttons* buttons;
};
}

#endif  // DAYDREAMIMPLEMENTATION_CONTROLLERCLICKEVENT_H
