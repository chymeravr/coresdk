#include <ControllerDaydream.h>
#include <android/log.h>

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
ControllerDaydream::ControllerDaydream(gvr::GvrApi *gvr_api,
                                       ILoggerFactory *loggerFactory)
    : gvr_api_(gvr_api) {
  this->logger = loggerFactory->createLogger("ControllerDaydream: ");
}

ControllerDaydream::~ControllerDaydream() {}

// TODO : transform is probably best to be put inside the constructor
void ControllerDaydream::setControllerTransform(
    TransformTreeModel &controllerTransformTree) {
  this->controllerTransformTree = &controllerTransformTree;
}

void ControllerDaydream::updateController() {
  gvr::ClockTimePoint target_time = gvr::GvrApi::GetTimePointNow();
  target_time.monotonic_system_time_nanos += kPredictionTimeWithoutVsyncNanos;

  this->head_view_ =
      this->gvr_api_->GetHeadSpaceFromStartSpaceRotation(target_time);

  if (this->controllerTransformTree == nullptr) {
    return;
  }

  if (!this->gvr_controller_api_) {
    return;
  }

  // Get Controller Position and Orientation
  auto controllerOrientationQuat = this->gvr_controller_state_.GetOrientation();
  auto controllerPosition = this->gvr_controller_state_.GetPosition();

  // Update controller transform with orientation data
  CL_Quat controllerQuat =
      CL_Quat(controllerOrientationQuat.qw, controllerOrientationQuat.qx,
              controllerOrientationQuat.qy, controllerOrientationQuat.qz);
  this->controllerTransformTree->setLocalQuaternion(controllerQuat);

  // Update controller transform with position data (arm model determines this)
  auto controllerPos =
      CL_Vec3(controllerPosition.x, controllerPosition.y, controllerPosition.z);
  this->controllerTransformTree->setLocalPosition(controllerPos);
}

void ControllerDaydream::ResumeControllerApiAsNeeded() {
  if (!gvr_controller_api_) {
    // Initialized controller api.
    gvr_controller_api_.reset(new gvr::ControllerApi);

    // Set up the options:
    int32_t options = gvr::ControllerApi::DefaultOptions();

    // Enable non-default options : Arm Model & Position Information
    options |= GVR_CONTROLLER_ENABLE_ARM_MODEL | GVR_CONTROLLER_ENABLE_POSITION;
    CHECK(gvr_controller_api_);
    CHECK(gvr_controller_api_->Init(options, gvr_api_->cobj()));
  }
  gvr_controller_api_->Resume();
}

void ControllerDaydream::ProcessControllerInput() {
  const int old_status = gvr_controller_state_.GetApiStatus();
  const int old_connection_state = gvr_controller_state_.GetConnectionState();

  gvr_controller_api_->ApplyArmModel(this->handedness, this->behavior,
                                     this->head_view_);
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
}