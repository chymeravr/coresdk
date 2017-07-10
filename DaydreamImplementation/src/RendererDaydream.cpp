//
// Created by robin_chimera on 1/9/2017.
// Contains an implementation for GearVR HMD using the oculus mobile SDK (ovr)
// for android
//

#include <RendererDaydream.h>
#include <coreEngine/components/transformTree/TransformTreeCamera.h>
#include <coreEngine/renderObjects/Material.h>
#include <coreEngine/renderObjects/Model.h>
#include <coreEngine/renderObjects/Shader.h>
#include <glImplementation/renderObjects/CameraGL.h>
#include <stdlib.h>

#include <android/log.h>
#include <coreEngine/components/transformTree/TransformTreeModel.h>
#include <random>

// todo: replace with out own logger
#define LOG_TAG "RendererDaydream"
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define CHECK(condition)                                                   \
  if (!(condition)) {                                                      \
    LOGE("*** CHECK FAILED at %s:%d: %s", __FILE__, __LINE__, #condition); \
    abort();                                                               \
  }

namespace cl {
typedef enum { NO_EVENT = 0, NOTIFY_ME = 1, CLOSE_AD = 2 } keyEventResponse;

static const float kZNear = 1.0f;
static const float kZFar = 1000.0f;

static const uint64_t kPredictionTimeWithoutVsyncNanos = 50000000;

static std::array<float, 16> MatrixToGLArray(const gvr::Mat4f &matrix) {
  // Note that this performs a *tranpose* to a column-major matrix array, as
  // expected by GL.
  std::array<float, 16> result;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result[j * 4 + i] = matrix.m[i][j];
    }
  }
  return result;
}

static gvr::Rectf ModulateRect(const gvr::Rectf &rect, float width,
                               float height) {
  gvr::Rectf result = {rect.left * width, rect.right * width,
                       rect.bottom * height, rect.top * height};
  return result;
}

static gvr::Recti CalculatePixelSpaceRect(const gvr::Sizei &texture_size,
                                          const gvr::Rectf &texture_rect) {
  const float width = static_cast<float>(texture_size.width);
  const float height = static_cast<float>(texture_size.height);
  const gvr::Rectf rect = ModulateRect(texture_rect, width, height);
  const gvr::Recti result = {
      static_cast<int>(rect.left), static_cast<int>(rect.right),
      static_cast<int>(rect.bottom), static_cast<int>(rect.top)};
  return result;
}

static void CheckGLError(const char *label) {
  int gl_error = glGetError();
  if (gl_error != GL_NO_ERROR) {
    LOGW("GL error @ %s: %d", label, gl_error);
    // Crash immediately to make OpenGL errors obvious.
    abort();
  }
}

static gvr::Sizei HalfPixelCount(const gvr::Sizei &in) {
  // Scale each dimension by sqrt(2)/2 ~= 7/10ths.
  gvr::Sizei out;
  out.width = (7 * in.width) / 10;
  out.height = (7 * in.height) / 10;
  return out;
}

static gvr::Mat4f ControllerQuatToMatrix(const gvr::ControllerQuat &quat) {
  gvr::Mat4f result;
  const float x = quat.qx;
  const float x2 = quat.qx * quat.qx;
  const float y = quat.qy;
  const float y2 = quat.qy * quat.qy;
  const float z = quat.qz;
  const float z2 = quat.qz * quat.qz;
  const float w = quat.qw;
  const float xy = quat.qx * quat.qy;
  const float xz = quat.qx * quat.qz;
  const float xw = quat.qx * quat.qw;
  const float yz = quat.qy * quat.qz;
  const float yw = quat.qy * quat.qw;
  const float zw = quat.qz * quat.qw;

  const float m11 = 1.0f - 2.0f * y2 - 2.0f * z2;
  const float m12 = 2.0f * (xy - zw);
  const float m13 = 2.0f * (xz + yw);
  const float m21 = 2.0f * (xy + zw);
  const float m22 = 1.0f - 2.0f * x2 - 2.0f * z2;
  const float m23 = 2.0f * (yz - xw);
  const float m31 = 2.0f * (xz - yw);
  const float m32 = 2.0f * (yz + xw);
  const float m33 = 1.0f - 2.0f * x2 - 2.0f * y2;

  return {{{m11, m12, m13, 0.0f},
           {m21, m22, m23, 0.0f},
           {m31, m32, m33, 0.0f},
           {0.0f, 0.0f, 0.0f, 1.0f}}};
}

static inline float VectorNorm(const std::array<float, 4> &vect) {
  return std::sqrt(vect[0] * vect[0] + vect[1] * vect[1] + vect[2] * vect[2]);
}

static float VectorInnerProduct(const std::array<float, 4> &vect1,
                                const std::array<float, 4> &vect2) {
  float product = 0;
  for (int i = 0; i < 3; i++) {
    product += vect1[i] * vect2[i];
  }
  return product;
}

static gvr::Mat4f MatrixMul(const gvr::Mat4f &matrix1,
                            const gvr::Mat4f &matrix2) {
  gvr::Mat4f result;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result.m[i][j] = 0.0f;
      for (int k = 0; k < 4; ++k) {
        result.m[i][j] += matrix1.m[i][k] * matrix2.m[k][j];
      }
    }
  }
  return result;
}

static std::array<float, 4> MatrixVectorMul(const gvr::Mat4f &matrix,
                                            const std::array<float, 4> &vec) {
  std::array<float, 4> result;
  for (int i = 0; i < 4; ++i) {
    result[i] = 0;
    for (int k = 0; k < 4; ++k) {
      result[i] += matrix.m[i][k] * vec[k];
    }
  }
  return result;
}

void RendererDaydream::ResumeControllerApiAsNeeded() {
  switch (gvr_viewer_type_) {
    case GVR_VIEWER_TYPE_CARDBOARD:
      gvr_controller_api_.reset();
      break;
    case GVR_VIEWER_TYPE_DAYDREAM:
      if (!gvr_controller_api_) {
        // Initialized controller api.
        gvr_controller_api_.reset(new gvr::ControllerApi);
        CHECK(gvr_controller_api_);
        CHECK(gvr_controller_api_->Init(gvr::ControllerApi::DefaultOptions(),
                                        gvr_api_->cobj()));
      }
      gvr_controller_api_->Resume();
      break;
    default:
      LOGE("unexpected viewer type.");
      break;
  }
}

void RendererDaydream::ProcessControllerInput() {
  const int old_status = gvr_controller_state_.GetApiStatus();
  const int old_connection_state = gvr_controller_state_.GetConnectionState();

  // Read current controller state.
  gvr_controller_state_.Update(*gvr_controller_api_);

  // Print new API status and connection state, if they changed.
  if (gvr_controller_state_.GetApiStatus() != old_status ||
      gvr_controller_state_.GetConnectionState() != old_connection_state) {
    LOGD("RendererDaydream: controller API status: %s, connection state: %s",
         gvr_controller_api_status_to_string(
             gvr_controller_state_.GetApiStatus()),
         gvr_controller_connection_state_to_string(
             gvr_controller_state_.GetConnectionState()));
  }

  // Trigger click event if app/click button is clicked.
  if (gvr_controller_state_.GetButtonDown(GVR_CONTROLLER_BUTTON_APP) ||
      gvr_controller_state_.GetButtonDown(GVR_CONTROLLER_BUTTON_CLICK)) {
    // todo : bind the click events to close and notify me
    logger->log(LOG_DEBUG, "Controller Button Click");
    // this->OnTriggerEvent();
  }
}

void RendererDaydream::updateController(Scene *scene) {
  // TODO - remove this from renderer. hand over controller to activity class
  // initialize controller
  if (gvr_viewer_type_ == GVR_VIEWER_TYPE_DAYDREAM) {
    auto controller4scene = (Model *)scene->getFromScene("controllerModel");
    auto controllerTransform =
        (TransformTreeModel *)controller4scene->getComponentList().getComponent(
            "transformTree");
    auto controllerOrientationQuat = gvr_controller_state_.GetOrientation();
    CL_Quat controllerQuat =
        CL_Quat(controllerOrientationQuat.qw, controllerOrientationQuat.qx,
                controllerOrientationQuat.qy, controllerOrientationQuat.qz);
    auto controllerPositionGvr = gvr_controller_state_.GetPosition();
    // LOGD("Controller Quat : %f %f %f %f", controllerQuat.x, controllerQuat.y,
    // controllerQuat.z, controllerQuat.w);
    // LOGD("Controller Positions : %f %f %f", controllerPositionGvr.x,
    // controllerPositionGvr.y, controllerPositionGvr.z);
    controllerTransform->setLocalQuaternion(controllerQuat);
    //            auto controllerPosition = CL_Vec3(controllerPositionGvr.x,
    //            controllerPositionGvr.y, controllerPositionGvr.z);
    //            controllerTransform->setLocalPosition(controllerPosition);
  }
}

RendererDaydream::RendererDaydream(gvr_context *gvr_context,
                                   ILoggerFactory *loggerFactory)
    : gvr_api_(gvr::GvrApi::WrapNonOwned(gvr_context)),
      scratch_viewport_(gvr_api_->CreateBufferViewport()),
      gvr_viewer_type_(gvr_api_->GetViewerType()) {
  ResumeControllerApiAsNeeded();
  if (gvr_viewer_type_ == GVR_VIEWER_TYPE_CARDBOARD) {
    LOGD("Viewer type: CARDBOARD");
  } else if (gvr_viewer_type_ == GVR_VIEWER_TYPE_DAYDREAM) {
    LOGD("Viewer type: DAYDREAM");
  } else {
    LOGE("Unexpected viewer type.");
  }
  // todo: remove hardcoded string
  logger = loggerFactory->createLogger("RendererDaydream");
}

// todo : figure out if we have to do any cleanup
RendererDaydream::~RendererDaydream() {}

bool RendererDaydream::start() { return true; }

bool RendererDaydream::initialize(Scene *scene) {
  logger->log(LOG_DEBUG, "Renderer Intialization Start!!!");
  InitializeGl();

  frame = (gvr::Frame *)malloc(sizeof(gvr::Frame));

  IRenderable *sceneRenderer = scene->getRenderable();
  sceneRenderer->initialize();
  std::vector<Relation *> cameraRelations = scene->getRelations("camera");
  assert(cameraRelations.size() == 1);
  ((Camera *)cameraRelations[0])->getRenderable()->initialize();

  std::vector<Relation *> shaderRelations = scene->getRelations("shader");
  for (auto it = shaderRelations.cbegin(); it != shaderRelations.cend(); it++) {
    Shader *shader = (Shader *)(*it);
    shader->getRenderable()->initialize();

    std::vector<Relation *> materialRelations =
        shader->getRelations("material");
    for (auto it = materialRelations.cbegin(); it != materialRelations.cend();
         it++) {
      Material *material = (Material *)(*it);
      material->getRenderable()->initialize();

      std::vector<Relation *> modelRelations = material->getRelations("model");
      for (auto it = modelRelations.cbegin(); it != modelRelations.cend();
           it++) {
        Model *model = (Model *)(*it);
        model->getRenderable()->initialize();
      }
    }
  }

  this->renderCamera = (CameraGL *)cameraRelations[0];
  this->renderCamera->setIsAsymetricProjection(true);

  this->renderCamera->setNearPlane(kZNear);
  this->renderCamera->setFarPlane(kZFar);

  this->updateController(scene);
  logger->log(LOG_DEBUG, "Renderer Intialization Complete!!!");
  return true;
}

void RendererDaydream::update() {}

void RendererDaydream::drawInit(Scene *scene) {
  if (gvr_viewer_type_ == GVR_VIEWER_TYPE_DAYDREAM) {
    ProcessControllerInput();
  }

  PrepareFramebuffer();
  // gvr::Frame frame = swapchain_->AcquireFrame();
  *this->frame = swapchain_->AcquireFrame();

  // A client app does its rendering here.
  gvr::ClockTimePoint target_time = gvr::GvrApi::GetTimePointNow();
  target_time.monotonic_system_time_nanos += kPredictionTimeWithoutVsyncNanos;

  this->head_view_ =
      this->gvr_api_->GetHeadSpaceFromStartSpaceRotation(target_time);

  TransformTreeCamera *transform =
      (TransformTreeCamera *)this->renderCamera->getComponentList()
          .getComponent("transformTree");

  auto matArray = MatrixToGLArray(head_view_);
  auto rotMat = CL_Mat44(matArray[0], matArray[4], matArray[8], matArray[12],
                         matArray[1], matArray[5], matArray[9], matArray[13],
                         matArray[2], matArray[6], matArray[10], matArray[14],
                         matArray[3], matArray[7], matArray[11], matArray[15]);

  this->rotQuat = CL_Rot_To_Quat(rotMat);
  transform->setLocalQuaternion(this->rotQuat);
  auto cameraQuat = transform->getLocalQuaternion();
  //  LOGD("Camera Params : %f %f %f %f", cameraQuat.x, cameraQuat.y,
  //  cameraQuat.z, cameraQuat.w);
  // A client app does its rendering here.

  this->viewport_list_->SetToRecommendedBufferViewports();

  this->frame->BindBuffer(0);
  glClearColor(0.1f, 0.1f, 0.1f, 0.5f);  // Dark background so text shows up.

  IRenderable *sceneRenderer = scene->getRenderable();
  sceneRenderer->draw();
}

void RendererDaydream::drawLeft(Scene *scene) {
  TransformTreeCamera *transform =
      (TransformTreeCamera *)this->renderCamera->getComponentList()
          .getComponent("transformTree");
  auto cameraQuat = transform->getLocalQuaternion();
  //  LOGD("Camera Params : %f %f %f %f", cameraQuat.x, cameraQuat.y,
  //  cameraQuat.z, cameraQuat.w);
  this->viewport_list_->GetBufferViewport(0, &this->scratch_viewport_);
  gvr::Mat4f eye_matrix = gvr_api_->GetEyeFromHeadMatrix(GVR_LEFT_EYE);

  auto left_camera_position = CL_Vec3(-1 * eye_matrix.m[0][3],
                                      -eye_matrix.m[1][3], -eye_matrix.m[2][3]);

  {
    auto fovs = this->scratch_viewport_.GetSourceFov();

    auto left = -std::tan(fovs.left * M_PI / 180.0f) * kZNear;
    auto right = std::tan(fovs.right * M_PI / 180.0f) * kZNear;
    auto bottom = -std::tan(fovs.bottom * M_PI / 180.0f) * kZNear;
    auto top = std::tan(fovs.top * M_PI / 180.0f) * kZNear;

    this->renderCamera->setLeft(left);
    this->renderCamera->setRight(right);
    this->renderCamera->setTop(top);
    this->renderCamera->setBottom(bottom);

    transform->setLocalPosition(left_camera_position);
  }

  gvr::Recti pixel_rect = CalculatePixelSpaceRect(
      this->render_size_, this->scratch_viewport_.GetSourceUv());

  glViewport(pixel_rect.left, pixel_rect.bottom,
             pixel_rect.right - pixel_rect.left,
             pixel_rect.top - pixel_rect.bottom);

  CheckGLError("World drawing setup");

  drawScene(scene);
}

void RendererDaydream::drawRight(Scene *scene) {
  TransformTreeCamera *transform =
      (TransformTreeCamera *)this->renderCamera->getComponentList()
          .getComponent("transformTree");
  transform->setLocalQuaternion(this->rotQuat);

  auto cameraQuat = transform->getLocalQuaternion();
  //  LOGD("Camera Params : %f %f %f %f", cameraQuat.x, cameraQuat.y,
  //  cameraQuat.z, cameraQuat.w);
  this->viewport_list_->GetBufferViewport(1, &this->scratch_viewport_);
  gvr::Mat4f eye_matrix = gvr_api_->GetEyeFromHeadMatrix(GVR_RIGHT_EYE);

  auto right_camera_position = CL_Vec3(
      -1 * eye_matrix.m[0][3], -eye_matrix.m[1][3], -eye_matrix.m[2][3]);

  {
    auto fovs = this->scratch_viewport_.GetSourceFov();

    auto left = -std::tan(fovs.left * M_PI / 180.0f) * kZNear;
    auto right = std::tan(fovs.right * M_PI / 180.0f) * kZNear;
    auto bottom = -std::tan(fovs.bottom * M_PI / 180.0f) * kZNear;
    auto top = std::tan(fovs.top * M_PI / 180.0f) * kZNear;

    this->renderCamera->setLeft(left);
    this->renderCamera->setRight(right);
    this->renderCamera->setTop(top);
    this->renderCamera->setBottom(bottom);

    transform->setLocalPosition(right_camera_position);
  }

  gvr::Recti pixel_rect = CalculatePixelSpaceRect(
      this->render_size_, this->scratch_viewport_.GetSourceUv());

  glViewport(pixel_rect.left, pixel_rect.bottom,
             pixel_rect.right - pixel_rect.left,
             pixel_rect.top - pixel_rect.bottom);

  CheckGLError("World drawing setup");

  drawScene(scene);
}

void RendererDaydream::drawComplete() {
  this->frame->Unbind();

  this->frame->Submit(*viewport_list_, head_view_);

  CheckGLError("onDrawFrame");
}

void RendererDaydream::deinitialize(Scene *scene) { free(frame); }

void RendererDaydream::stop() {}

void RendererDaydream::pause() {
  gvr_api_->PauseTracking();
  if (gvr_controller_api_) gvr_controller_api_->Pause();
}

void RendererDaydream::resume() {
  gvr_api_->ResumeTracking();
  ResumeControllerApiAsNeeded();
}

std::vector<float> RendererDaydream::getHMDParams() {
  TransformTreeCamera *transform =
      (TransformTreeCamera *)this->renderCamera->getComponentList()
          .getComponent("transformTree");
  auto quat = transform->getLocalQuaternion();
  std::vector<float> result;

  result.push_back(quat[0]);
  result.push_back(quat[1]);
  result.push_back(quat[2]);
  result.push_back(quat[3]);

  return result;
}

/*************************************************************************************************************************
     *************************************************************************************************************************/

void RendererDaydream::PrepareFramebuffer() {
  // Because we are using 2X MSAA, we can render to half as many pixels and
  // achieve similar quality.
  const gvr::Sizei recommended_size =
      HalfPixelCount(gvr_api_->GetMaximumEffectiveRenderTargetSize());
  if (render_size_.width != recommended_size.width ||
      render_size_.height != recommended_size.height) {
    // We need to resize the framebuffer.
    swapchain_->ResizeBuffer(0, recommended_size);
    render_size_ = recommended_size;
  }
}

void RendererDaydream::InitializeGl() {
  logger->log(LOG_DEBUG, "Initializing Gvr Api!!!");
  gvr_api_->InitializeGl();

  // Because we are using 2X MSAA, we can render to half as many pixels and
  // achieve similar quality.
  render_size_ =
      HalfPixelCount(gvr_api_->GetMaximumEffectiveRenderTargetSize());
  std::vector<gvr::BufferSpec> specs;

  specs.push_back(gvr_api_->CreateBufferSpec());
  specs[0].SetColorFormat(GVR_COLOR_FORMAT_RGBA_8888);
  specs[0].SetDepthStencilFormat(GVR_DEPTH_STENCIL_FORMAT_DEPTH_16);
  specs[0].SetSize(render_size_);
  specs[0].SetSamples(2);

  swapchain_.reset(new gvr::SwapChain(gvr_api_->CreateSwapChain(specs)));

  viewport_list_.reset(
      new gvr::BufferViewportList(gvr_api_->CreateEmptyBufferViewportList()));
  logger->log(LOG_DEBUG, "Initializing Gvr Api Complete!!!");
}

void RendererDaydream::drawScene(Scene *scene) {
  IRenderable *sceneRenderer = scene->getRenderable();
  // sceneRenderer->draw();

  std::vector<Relation *> cameraRelations = scene->getRelations("camera");
  assert(cameraRelations.size() == 1);
  ((Camera *)cameraRelations[0])->getRenderable()->draw();

  std::vector<Relation *> shaderRelations = scene->getRelations("shader");
  for (auto it = shaderRelations.cbegin(); it != shaderRelations.cend(); it++) {
    Shader *shader = (Shader *)(*it);
    shader->getRenderable()->draw();

    std::vector<Relation *> materialRelations =
        shader->getRelations("material");
    for (auto it = materialRelations.cbegin(); it != materialRelations.cend();
         it++) {
      Material *material = (Material *)(*it);
      material->getRenderable()->draw();

      std::vector<Relation *> modelRelations = material->getRelations("model");
      for (auto it = modelRelations.cbegin(); it != modelRelations.cend();
           it++) {
        Model *model = (Model *)(*it);
        model->getRenderable()->draw();
      }
    }
  }

  if (gvr_viewer_type_ == GVR_VIEWER_TYPE_DAYDREAM) {
    //    auto gvrReticleQuat = gvr_controller_state_.GetOrientation();
    //    CL_Quat reticleQuat = CL_Quat(gvrReticleQuat.qw, gvrReticleQuat.qx,
    //                                  gvrReticleQuat.qy, gvrReticleQuat.qz);
    //    auto reticle4scene = (Model *)scene->getFromScene("reticle");
    //    auto reticleTransform =
    //        (TransformTreeModel
    //        *)reticle4scene->getComponentList().getComponent(
    //            "transformTree");
    //    // LOGD("Quaternion parameters : %f, %f, %f, %f", gvrReticleQuat.qw,
    //    // gvrReticleQuat.qx, gvrReticleQuat.qy, gvrReticleQuat.qz);
    //    // auto rotmat = CL_RotationMatrix(reticleQuat);
    //    auto reticleBaseTransform =
    //        (TransformTreeModel *)reticleTransform->getParent();
    //    reticleBaseTransform->setLocalQuaternion(reticleQuat);
  }

  this->updateController(scene);
}
}