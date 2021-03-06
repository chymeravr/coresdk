//
// Created by robin_chimera on 1/9/2017.
//

#ifndef CARDBOARDIMPLEMENTATION_RENDERER_H
#define CARDBOARDIMPLEMENTATION_RENDERER_H

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <jni.h>

#include <memory>
#include <string>
#include <thread> // NOLINT
#include <vector>

#include "vr/gvr/capi/include/gvr.h"
//#include "vr/gvr/capi/include/gvr_audio.h"
//#include "vr/gvr/capi/include/gvr_controller.h"
#include "vr/gvr/capi/include/gvr_types.h"

#include <coreEngine/IRenderer.h>
#include <glImplementation/renderObjects/CameraGL.h>

namespace cl
{

class RendererCardboard : public IRenderer
{

private:
  void PrepareFramebuffer();

  gvr::Frame *frame;

  // gvr stuff

  std::unique_ptr<gvr::GvrApi> gvr_api_;
  std::unique_ptr<gvr::BufferViewportList> viewport_list_;
  std::unique_ptr<gvr::SwapChain> swapchain_;
  gvr::BufferViewport scratch_viewport_;

  gvr::Mat4f head_view_;
  gvr::Sizei render_size_;

  gvr::ViewerType gvr_viewer_type_;

  void InitializeGl();

  CameraGL *renderCamera;

  ILoggerFactory *loggerFactory;

  std::unique_ptr<ILogger> logger;

public:
  RendererCardboard(gvr_context *gvr_context, ILoggerFactory *loggerFactory);

  ~RendererCardboard();

  bool start();

  bool initialize(Scene *scene);

  void update();

  void drawInit(Scene *scene);

  void draw(Scene *scene, EYE eye);

  void drawComplete();

  void deinitialize(Scene *scene);

  void stop();

  void pause();

  void resume();

  std::vector<float> getHMDParams();

  void drawScene(Scene *scene);
};
}

#endif //CARDBOARDIMPLEMENTATION_RENDERER_H
