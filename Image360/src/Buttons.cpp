#include <image360/Buttons.h>

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
}
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
      "actionButton", &scene, ACTION_BUTTON_COLOR, ACTION_BUTTON_POSITION,
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
      "closeButton", &scene, CLOSE_BUTTON_COLOR, CLOSE_BUTTON_POSITION,
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
  this->actionButtonListener =
      this->eventGazeListenerFactory->createActionButtonListener();

  // initializing close button model
  Model *closeButtonModel = (Model *)scene.getFromScene("closeButton");
  TransformTreeModel *transformCloseButton =
      (TransformTreeModel *)closeButtonModel->getComponentList().getComponent(
          "transformTree");
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
}