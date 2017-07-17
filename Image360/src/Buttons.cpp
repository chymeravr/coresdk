#include <image360/Buttons.h>

namespace cl {
Buttons::Buttons(ILoggerFactory &loggerFactory, UIFactory &uiFactory,
                 GazeDetectorFactory &gazeDetectorFactory, FontStore &fontStore,
                 IEventGazeListenerFactory &eventGazeListenerFactory) {
  this->uiFactory = &uiFactory;
  this->gazeDetectorFactory = &gazeDetectorFactory;
  this->eventGazeListenerFactory = &eventGazeListenerFactory;
  this->fontStore = &fontStore;
  this->logger = loggerFactory.createLogger("Image360:Buttons:: ");
}
void Buttons::initialize(Scene &scene, TransformTree &gazeTransformTarget) {
  // Text style info
  TextStyle textStyle;
  textStyle.fontSize = 20;
  textStyle.scale = 0.025f;
  textStyle.color = CL_Vec4(1.0, 1.0, 1.0, 1.0);

  // Action Button Initialization
  auto actionButtonPosition = CL_Vec3(-5.1, 0.0, -15.5);
  auto actionButtonRotation = CL_Vec3(0.0, 0.0, 0.0);
  auto actionButtonColor = CL_Vec4(0.0, 0.0, 0.0, 0.7);
  auto actionButtonWidth = 3.0f;
  auto actionButtonHeight = 1.0f;
  this->actionButtonBackground = this->uiFactory->createPlanarBackground(
      "actionButton", &scene, actionButtonColor, actionButtonPosition,
      actionButtonRotation, actionButtonWidth, actionButtonHeight);

  // action button text position & orientation is relative to the actionButton
  // planar background
  auto actionTextPosition = CL_Vec3(-1.0, -0.2, 0.001);
  auto actionTextRotation = CL_Vec3(0.0, 0.0, 0.0);
  std::unique_ptr<TextElement> actionButtonElement =
      this->uiFactory->createTextElement(
          "actionButtonElement", fontStore, &textStyle, this->actionButtonText,
          actionTextPosition, actionTextRotation, &scene);
  this->actionButtonBackground->addChild("child1",
                                         std::move(actionButtonElement));

  // Close Button Intialization
  auto closeButtonPosition = CL_Vec3(5.1, 0.0, -15.5);
  auto closeButtonRotation = CL_Vec3(0.0, 0.0, 0.0);
  auto closeButtonColor = CL_Vec4(0.0, 0.0, 0.0, 0.7);
  auto closeButtonWidth = 3.0f;
  auto closeButtonHeight = 1.0f;
  this->closeButtonBackground = this->uiFactory->createPlanarBackground(
      "closeButton", &scene, closeButtonColor, closeButtonPosition,
      closeButtonRotation, closeButtonWidth, closeButtonHeight);

  // close button text position & orientation is relative to the actionButton
  // planar background
  auto closeTextPosition = CL_Vec3(-0.5, -0.2, 0.001);
  auto closeTextRotation = CL_Vec3(0.0, 0.0, 0.0);
  std::unique_ptr<TextElement> closeButtonElement =
      this->uiFactory->createTextElement(
          "closeButtonElement", fontStore, &textStyle, this->closeButtonText,
          closeTextPosition, closeTextRotation, &scene);
  this->closeButtonBackground->addChild("child2",
                                        std::move(closeButtonElement));

  this->gazeDetectorContainer =
      this->gazeDetectorFactory->createGazeDetectorContainer();

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
  auto actionButtonGazeDetectorOrigin = CL_Vec3(0.0f, 0.0f, 0.0f);
  auto actionButtonGazeDetectorLookAt = CL_Vec3(0.0f, 0.0f, -1.0f);
  auto actionButtonGazeDetectorLengthX = 3.0f;
  auto actionButtonGazeDetectorLengthY = 1.0f;
  auto actionButtonGazeDetectorLengthZ = 0.00001f;
  std::unique_ptr<IComponent> actionButtonGazeDetector =
      this->gazeDetectorFactory->createGazeDetectorBox(
          this->actionButtonText, &gazeTransformTarget, transformActionButton,
          actionButtonListener.get(), gazeDetectorContainer.get(),
          actionButtonGazeDetectorOrigin, actionButtonGazeDetectorLookAt,
          actionButtonGazeDetectorLengthX, actionButtonGazeDetectorLengthY,
          actionButtonGazeDetectorLengthZ);
  actionButtonModel->getComponentList().addComponent(
      std::move(actionButtonGazeDetector));

  auto closeButtonGazeDetectorOrigin = CL_Vec3(0.0f, 0.0f, 0.0f);
  auto closeButtonGazeDetectorLookAt = CL_Vec3(0.0f, 0.0f, -1.0f);
  auto closeButtonGazeDetectorLengthX = 3.0f;
  auto closeButtonGazeDetectorLengthY = 1.0f;
  auto closeButtonGazeDetectorLengthZ = 0.00001f;
  std::unique_ptr<IComponent> closeButtonGazeDetector =
      this->gazeDetectorFactory->createGazeDetectorBox(
          this->closeButtonText, &gazeTransformTarget, transformCloseButton,
          closeButtonListener.get(), gazeDetectorContainer.get(),
          closeButtonGazeDetectorOrigin, closeButtonGazeDetectorLookAt,
          closeButtonGazeDetectorLengthX, closeButtonGazeDetectorLengthY,
          closeButtonGazeDetectorLengthZ);
  closeButtonModel->getComponentList().addComponent(
      std::move(closeButtonGazeDetector));
}
}