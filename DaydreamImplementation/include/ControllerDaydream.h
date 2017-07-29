//
// Created by robin_chimera on 1/9/2017.
//

#ifndef DAYDREAMIMPLEMENTATION_CONTROLLER_H
#define DAYDREAMIMPLEMENTATION_CONTROLLER_H

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <jni.h>

#include <memory>
#include <string>
#include <thread>  // NOLINT
#include <vector>

#include "vr/gvr/capi/include/gvr.h"
#include "vr/gvr/capi/include/gvr_controller.h"
#include "vr/gvr/capi/include/gvr_types.h"

#include <coreEngine/components/transformTree/TransformTreeModel.h>
#include <coreEngine/events/IEvent.h>

namespace cl {

class ControllerDaydream {
 private:
  // Controller API entry point.
  std::unique_ptr<gvr::ControllerApi> gvr_controller_api_;

  // todo - get these params from UserPref object
  gvr::ControllerHandedness handedness =
      static_cast<gvr::ControllerHandedness>(GVR_CONTROLLER_RIGHT_HANDED);
  gvr::ArmModelBehavior behavior =
      static_cast<gvr::ArmModelBehavior>(GVR_ARM_MODEL_IGNORE_GAZE);

  // The latest controller state (updated once per frame).
  gvr::ControllerState gvr_controller_state_;

  // gvr stuff

  gvr::GvrApi *gvr_api_;
  gvr::Mat4f head_view_;

  ILoggerFactory *loggerFactory;
  std::unique_ptr<ILogger> logger;
  IEvent *controllerClickEventHandler = nullptr;

  // Controller Params
  TransformTreeModel *controllerTransformTree = nullptr;

  static const uint64_t kPredictionTimeWithoutVsyncNanos = 50000000;

 public:
  ControllerDaydream(gvr::GvrApi *gvr_api, ILoggerFactory *loggerFactory);

  ~ControllerDaydream();

  void updateController();
  void ResumeControllerApiAsNeeded();
  void ProcessControllerInput();

  //   bool start();

  //   bool initialize(Scene *scene);

  //   void update();

  //   void drawInit(Scene *scene);

  //   void drawLeft(Scene *scene);

  //   void drawRight(Scene *scene);

  //   void drawComplete();

  //   void deinitialize(Scene *scene);

  //   void stop();

  void pause();

  void resume();

  void setControllerTransform(TransformTreeModel &controllerTransformTree);
  void setControllerClickEventHandler(IEvent &event) {
    this->controllerClickEventHandler = &event;
  }
};
}

#endif  // DAYDREAMIMPLEMENTATION_CONTROLLER_H
