//
// Created by robin_chimera on 1/9/2017.
//

#ifndef DAYDREAMIMPLEMENTATION_RENDERER_H
#define DAYDREAMIMPLEMENTATION_RENDERER_H

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <jni.h>

#include <memory>
#include <string>
#include <thread>  // NOLINT
#include <vector>

#include "vr/gvr/capi/include/gvr.h"
#include "vr/gvr/capi/include/gvr_audio.h"
#include "vr/gvr/capi/include/gvr_controller.h"
#include "vr/gvr/capi/include/gvr_types.h"

#include <coreEngine/IRenderer.h>
#include <coreEngine/components/transformTree/TransformTreeModel.h>
#include <glImplementation/renderObjects/CameraGL.h>

namespace cl {

class RendererDaydream : public IRenderer {
 private:
  void PrepareFramebuffer();
    void InitializeGl();

    // gvr stuff
  gvr::Frame *frame;
  gvr::GvrApi *gvr_api_;
  std::unique_ptr<gvr::BufferViewportList> viewport_list_;
  std::unique_ptr<gvr::SwapChain> swapchain_;
  gvr::BufferViewport scratch_viewport_;

  gvr::Mat4f head_view_;
  gvr::Sizei render_size_;

  ILoggerFactory *loggerFactory;
  std::unique_ptr<ILogger> logger;

  CameraGL *renderCamera;

  CL_Quat rotQuat;

 public:
  RendererDaydream(gvr::GvrApi *gvr_api, ILoggerFactory *loggerFactory);

  ~RendererDaydream();

  bool start();

  bool initialize(Scene *scene);

  void update();

  void drawInit(Scene *scene);

  void drawLeft(Scene *scene);

  void drawRight(Scene *scene);

  void drawComplete();

  void deinitialize(Scene *scene);

  void stop();

  void pause();

  void resume();

  std::vector<float> getHMDParams();

  // make private in future
  void drawScene(Scene *scene);
};
}

#endif  // DAYDREAMIMPLEMENTATION_RENDERER_H
