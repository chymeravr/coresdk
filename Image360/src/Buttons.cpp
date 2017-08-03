#include <image360/Buttons.h>

#include <coreEngine/animation/LinearInterpolator.h>
#include <coreEngine/animation/ChronoTimeKeeper.h>
#include <image360/ActionButtonListener.h>
#include <image360/CloseButtonListener.h>
#include <iostream>

namespace cl {
Buttons::Buttons(ILoggerFactory &loggerFactory, UIFactory &uiFactory,
                 GazeDetectorContainer &gazeDetectorContainer,
                 GazeDetectorFactory &gazeDetectorFactory,
                 IEventGazeListenerFactory &eventGazeListenerFactory,
                 TransformTree *gazeTransformShooter,
                 std::string fontFolderPath, IEvent &eventCloseApplication) {
  this->uiFactory = &uiFactory;
  this->gazeDetectorFactory = &gazeDetectorFactory;
  this->gazeDetectorContainer = &gazeDetectorContainer;
  this->eventGazeListenerFactory = &eventGazeListenerFactory;
  this->logger = loggerFactory.createLogger("Image360:Buttons:: ");
  this->gazeTransformShooter = gazeTransformShooter;
  this->fontFolderPath = fontFolderPath;
  this->eventCloseApplication = &eventCloseApplication;

  // ACTION BUTTON : color interpolation animation for on enter
  auto actionButtonColorInterpolatorOnEnter= std::unique_ptr<LinearInterpolator<CL_Vec4>>(
      new LinearInterpolator<CL_Vec4>());
  auto actionButtonColorTimeKeeperOnEnter = std::unique_ptr<ChronoTimeKeeper>(new ChronoTimeKeeper());
  this->actionButtonOnEnterColorAnimation =
      std::unique_ptr<Animation<CL_Vec4>>(new Animation<CL_Vec4>(
	  loggerFactory, this->actionButtonColor,
	  std::move(actionButtonColorInterpolatorOnEnter), std::move(actionButtonColorTimeKeeperOnEnter)));

  // ACTION BUTTON : size interpolation animation for on enter
  auto actionButtonScaleInterpolatorOnEnter = std::unique_ptr<LinearInterpolator<CL_Vec3>>(
	  new LinearInterpolator<CL_Vec3>());
  auto actionButtonScaleTimeKeeperOnEnter = std::unique_ptr<ChronoTimeKeeper>(new ChronoTimeKeeper());
  this->actionButtonOnEnterScaleAnimation =
	  std::unique_ptr<Animation<CL_Vec3>>(new Animation<CL_Vec3>(
	  loggerFactory, this->actionButtonScale,
	  std::move(actionButtonScaleInterpolatorOnEnter), std::move(actionButtonScaleTimeKeeperOnEnter)));

  // ACTION BUTTON : color interpolation animation for on leave
  auto actionButtonColorInterpolatorOnLeave = std::unique_ptr<LinearInterpolator<CL_Vec4>>(
	  new LinearInterpolator<CL_Vec4>());
  auto actionButtonColorTimeKeeperOnLeave = std::unique_ptr<ChronoTimeKeeper>(new ChronoTimeKeeper());
  this->actionButtonOnLeaveColorAnimation =
	  std::unique_ptr<Animation<CL_Vec4>>(new Animation<CL_Vec4>(
	  loggerFactory, this->actionButtonColor,
	  std::move(actionButtonColorInterpolatorOnLeave), std::move(actionButtonColorTimeKeeperOnLeave)));

  // ACTION BUTTON : size interpolation animation for on leave
  auto actionButtonScaleInterpolatorOnLeave = std::unique_ptr<LinearInterpolator<CL_Vec3>>(
	  new LinearInterpolator<CL_Vec3>());
  auto actionButtonScaleTimeKeeperOnLeave = std::unique_ptr<ChronoTimeKeeper>(new ChronoTimeKeeper());
  this->actionButtonOnLeaveScaleAnimation =
	  std::unique_ptr<Animation<CL_Vec3>>(new Animation<CL_Vec3>(
	  loggerFactory, this->actionButtonScale,
	  std::move(actionButtonScaleInterpolatorOnLeave), std::move(actionButtonScaleTimeKeeperOnLeave)));

  // CLOSE BUTTON : color interpolation animation for on enter
  auto closeButtonColorInterpolatorOnEnter = std::unique_ptr<LinearInterpolator<CL_Vec4>>(
	  new LinearInterpolator<CL_Vec4>());
  auto closeButtonColorTimeKeeperOnEnter = std::unique_ptr<ChronoTimeKeeper>(new ChronoTimeKeeper());
  this->closeButtonOnEnterColorAnimation =
	  std::unique_ptr<Animation<CL_Vec4>>(new Animation<CL_Vec4>(
	  loggerFactory, this->closeButtonColor,
	  std::move(closeButtonColorInterpolatorOnEnter), std::move(closeButtonColorTimeKeeperOnEnter)));

  // CLOSE BUTTON : size interpolation animation for on enter
  auto closeButtonScaleInterpolatorOnEnter = std::unique_ptr<LinearInterpolator<CL_Vec3>>(
	  new LinearInterpolator<CL_Vec3>());
  auto closeButtonScaleTimeKeeperOnEnter = std::unique_ptr<ChronoTimeKeeper>(new ChronoTimeKeeper());
  this->closeButtonOnEnterScaleAnimation =
	  std::unique_ptr<Animation<CL_Vec3>>(new Animation<CL_Vec3>(
	  loggerFactory, this->closeButtonScale,
	  std::move(closeButtonScaleInterpolatorOnEnter), std::move(closeButtonScaleTimeKeeperOnEnter)));

  // CLOSE BUTTON : color interpolation animation for on leave
  auto closeButtonColorInterpolatorOnLeave = std::unique_ptr<LinearInterpolator<CL_Vec4>>(
	  new LinearInterpolator<CL_Vec4>());
  auto closeButtonColorTimeKeeperOnLeave = std::unique_ptr<ChronoTimeKeeper>(new ChronoTimeKeeper());
  this->closeButtonOnLeaveColorAnimation =
	  std::unique_ptr<Animation<CL_Vec4>>(new Animation<CL_Vec4>(
	  loggerFactory, this->closeButtonColor,
	  std::move(closeButtonColorInterpolatorOnLeave), std::move(closeButtonColorTimeKeeperOnLeave)));

  // CLOSE BUTTON : size interpolation animation for on leave
  auto closeButtonSizeInterpolatorOnLeave = std::unique_ptr<LinearInterpolator<CL_Vec3>>(
	  new LinearInterpolator<CL_Vec3>());
  auto closeButtonScaleTimeKeeperOnLeave = std::unique_ptr<ChronoTimeKeeper>(new ChronoTimeKeeper());
  this->closeButtonOnLeaveScaleAnimation =
	  std::unique_ptr<Animation<CL_Vec3>>(new Animation<CL_Vec3>(
	  loggerFactory, this->closeButtonScale,
	  std::move(closeButtonSizeInterpolatorOnLeave), std::move(closeButtonScaleTimeKeeperOnLeave)));
}

Buttons::~Buttons() { this->logger->log(LOG_DEBUG, "Buttons Destructor"); }

void Buttons::initialize(Scene &scene) {
  // this->gazeTransformShooter = gazeTransformShooter;

  this->fontStore =
      uiFactory->createFontStore(&scene, this->fontFolderPath.c_str());

  // Text style info
  TextStyle textStyle;
  textStyle.fontSize = FONT_SIZE;
  textStyle.scale = FONT_SCALE;
  textStyle.color = FONT_COLOR;

  // Action Button Initialization
  this->actionButtonBackground = this->uiFactory->createPlanarBackground(
      "actionButton", &scene, this->actionButtonColor, ACTION_BUTTON_POSITION,
      ACTION_BUTTON_ROTATION, ACTION_BUTTON_WIDTH, ACTION_BUTTON_HEIGHT);

  // action button text position & orientation is relative to the actionButton
  // planar background
  std::unique_ptr<TextElement> actionButtonElement =
      this->uiFactory->createTextElement(
          "actionButtonElement", this->fontStore.get(), &textStyle,
          this->actionButtonText, ACTION_TEXT_POSITION, ACTION_TEXT_ROTATION,
          &scene);
  this->actionButtonBackground->addChild("child1",
                                         std::move(actionButtonElement));

  // Close Button Intialization
  this->closeButtonBackground = this->uiFactory->createPlanarBackground(
      "closeButton", &scene, this->closeButtonColor, CLOSE_BUTTON_POSITION,
      CLOSE_BUTTON_ROTATION, CLOSE_BUTTON_WIDTH, CLOSE_BUTTON_HEIGHT);

  // close button text position & orientation is relative to the actionButton
  // planar background
  std::unique_ptr<TextElement> closeButtonElement =
      this->uiFactory->createTextElement(
          "closeButtonElement", this->fontStore.get(), &textStyle,
          this->closeButtonText, CLOSE_TEXT_POSITION, CLOSE_TEXT_ROTATION,
          &scene);
  this->closeButtonBackground->addChild("child2",
                                        std::move(closeButtonElement));

  // initializing action button model
  Model *actionButtonModel = (Model *)scene.getFromScene("actionButton");
  TransformTreeModel *transformActionButton =
      (TransformTreeModel *)actionButtonModel->getComponentList().getComponent(
          "transformTree");

  // create an action button listener to handle events associated with it
  this->actionButtonListener =
      this->eventGazeListenerFactory->createActionButtonListener();

  // initializing close button model
  Model *closeButtonModel = (Model *)scene.getFromScene("closeButton");
  TransformTreeModel *transformCloseButton =
      (TransformTreeModel *)closeButtonModel->getComponentList().getComponent(
          "transformTree");

  // create a close button listener to handle events associated with it
  this->closeButtonListener =
      this->eventGazeListenerFactory->createCloseButtonListener();

  // gaze detectors for action and close button
  std::unique_ptr<IComponent> actionButtonGazeDetector =
      this->gazeDetectorFactory->createGazeDetectorBox(
          this->actionButtonText, this->gazeTransformShooter,
          transformActionButton, actionButtonListener.get(),
          gazeDetectorContainer, ACTION_BUTTON_GAZE_DETECTOR_ORIGIN,
          ACTION_BUTTON_GAZE_DETECTOR_LOOK_AT,
          ACTION_BUTTON_GAZE_DETECTOR_LENGTH_X,
          ACTION_BUTTON_GAZE_DETECTOR_LENGTH_Y,
          ACTION_BUTTON_GAZE_DETECTOR_LENGTH_Z);
  actionButtonModel->getComponentList().addComponent(
      std::move(actionButtonGazeDetector));

  std::unique_ptr<IComponent> closeButtonGazeDetector =
      this->gazeDetectorFactory->createGazeDetectorBox(
          this->closeButtonText, this->gazeTransformShooter,
          transformCloseButton, closeButtonListener.get(),
          gazeDetectorContainer, CLOSE_BUTTON_GAZE_DETECTOR_ORIGIN,
          CLOSE_BUTTON_GAZE_DETECTOR_LOOK_AT,
          CLOSE_BUTTON_GAZE_DETECTOR_LENGTH_X,
          CLOSE_BUTTON_GAZE_DETECTOR_LENGTH_Y,
          CLOSE_BUTTON_GAZE_DETECTOR_LENGTH_Z);
  closeButtonModel->getComponentList().addComponent(
      std::move(closeButtonGazeDetector));

  // ANIMATION INITIALIZATIONS - keyFrames, timeFrames
  // time frames for button animations
  std::vector<float> timeFrames = { BUTTON_ANIMATION_START_TIME, BUTTON_ANIMATION_END_TIME };

  // color animation key frame initialization for on enter
  std::vector<CL_Vec4> onEnterColorFrames = std::vector<CL_Vec4>{
	  this->ACTION_BUTTON_START_COLOR, this->ACTION_BUTTON_END_COLOR};
  
  // size animation key frames initialization for on enter
  std::vector<CL_Vec3> onEnterScaleFrames = std::vector<CL_Vec3>{
	  this->ACTION_BUTTON_START_SCALE, this->ACTION_BUTTON_END_SCALE};
  
  // color animation key frame initialization for on leave
  std::vector<CL_Vec4> onLeaveColorFrames = std::vector<CL_Vec4>{
	  this->ACTION_BUTTON_END_COLOR, this->ACTION_BUTTON_START_COLOR};

  // size animation key frames initialization for on enter
  std::vector<CL_Vec3> onLeaveScaleFrames = std::vector<CL_Vec3>{
	  this->ACTION_BUTTON_END_SCALE, this->ACTION_BUTTON_START_SCALE};
  

  /******************************* ACTION BUTTON *******************************************/
  this->actionButtonOnEnterColorAnimation->setKeyFrames(onEnterColorFrames, timeFrames);
  ((ActionButtonListener *)(this->actionButtonListener.get()))
	  ->setReticleEnterAnimation(*this->actionButtonOnEnterColorAnimation);
  
  this->actionButtonOnEnterScaleAnimation->setKeyFrames(onEnterScaleFrames, timeFrames);
  ((ActionButtonListener *)(this->actionButtonListener.get()))
	  ->setReticleEnterResizeAnimation(*this->actionButtonOnEnterScaleAnimation);

  this->actionButtonOnLeaveColorAnimation->setKeyFrames(onLeaveColorFrames, timeFrames);
  ((ActionButtonListener *)(this->actionButtonListener.get()))
	  ->setReticleLeaveAnimation(*this->actionButtonOnLeaveColorAnimation);

  this->actionButtonOnLeaveScaleAnimation->setKeyFrames(onLeaveScaleFrames, timeFrames);
  ((ActionButtonListener *)(this->actionButtonListener.get()))
	  ->setReticleLeaveResizeAnimation(*this->actionButtonOnLeaveScaleAnimation);

  /******************************* CLOSE BUTTON *******************************************/
  // CLOSE BUTTON : color animation key frame initialization for on enter
  this->closeButtonOnEnterColorAnimation->setKeyFrames(onEnterColorFrames, timeFrames);
  ((CloseButtonListener *)(this->closeButtonListener.get()))
	  ->setReticleEnterAnimation(*this->closeButtonOnEnterColorAnimation);

  // CLOSE BUTTON : size animation key frames initialization for on enter
  this->closeButtonOnEnterScaleAnimation->setKeyFrames(onEnterScaleFrames, timeFrames);
  ((CloseButtonListener *)(this->closeButtonListener.get()))
	  ->setReticleEnterResizeAnimation(*this->closeButtonOnEnterScaleAnimation);

  // CLOSE BUTTON : color animation key frame initialization for on leave
  this->closeButtonOnLeaveColorAnimation->setKeyFrames(onLeaveColorFrames, timeFrames);
  ((CloseButtonListener *)(this->closeButtonListener.get()))
	  ->setReticleLeaveAnimation(*this->closeButtonOnLeaveColorAnimation);

  // CLOSE BUTTON : size animation key frames initialization for on enter
  this->closeButtonOnLeaveScaleAnimation->setKeyFrames(onLeaveScaleFrames, timeFrames);
  ((CloseButtonListener *)(this->closeButtonListener.get()))
	  ->setReticleLeaveResizeAnimation(*this->closeButtonOnLeaveScaleAnimation);

}

void Buttons::onClickHandler() {
  if (this->actionButtonListener->inFocus()) {
    this->logger->log(LOG_DEBUG, "Action Button in Focus");
    return;
  }
  if (this->closeButtonListener->inFocus()) {
    this->logger->log(LOG_DEBUG, "Close Button in Focus");
    this->eventCloseApplication->callListener();
    return;
  }
  this->logger->log(LOG_DEBUG, "No Button in Focus");
}

void Buttons::preDraw(){
	if (this->actionButtonOnLeaveColorAnimation->isAnimationRunning()){
		this->actionButtonOnLeaveColorAnimation->update();
	}

	if (this->actionButtonOnLeaveScaleAnimation->isAnimationRunning()){
		this->actionButtonOnLeaveScaleAnimation->update();
	}

	if (this->closeButtonOnLeaveColorAnimation->isAnimationRunning()){
		this->closeButtonOnLeaveColorAnimation->update();
	}

	if (this->closeButtonOnLeaveScaleAnimation->isAnimationRunning()){
		this->closeButtonOnLeaveScaleAnimation->update();
	}

	this->actionButtonBackground->setColor(this->actionButtonColor);
	this->actionButtonBackground->setScale(this->actionButtonScale);

	this->closeButtonBackground->setColor(this->closeButtonColor);
	this->closeButtonBackground->setScale(this->closeButtonScale);
}
}