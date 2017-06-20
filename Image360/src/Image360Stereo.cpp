#include <assert.h>
#include <coreEngine/components/transformTree/TransformTreeFactory.h>
#include <coreEngine/modelBuilder/CubeBuilder.h>
#include <coreEngine/modelBuilder/UVSphereBuilder.h>
#include <coreEngine/modifier/ImageModifier.h>
#include <coreEngine/modifier/ModelModifier.h>
#include <coreEngine/util/SimpleOBJLoader.h>
#include <image360/Image360Stereo.h>

namespace cl {
Image360Stereo::Image360Stereo(
    std::unique_ptr<IRenderer> renderer,
    std::unique_ptr<ISceneFactory> sceneFactory,
    std::unique_ptr<IModelFactory> modelFactory,
    std::unique_ptr<IDiffuseTextureFactory> diffuseTextureFactory,
    std::unique_ptr<IDiffuseTextureCubeMapFactory> diffuseTextureCubeMapFactory,
    std::unique_ptr<ITransformTreeFactory> transformTreeFactory,
    std::unique_ptr<ICameraFactory> cameraFactory, IEventQueue *eventQueue,
    ILoggerFactory *loggerFactory, std::unique_ptr<UIFactory> uiFactory,
    std::unique_ptr<GazeDetectorFactory> gazeDetectorFactory,
    std::unique_ptr<IEventGazeListenerFactory> eventGazeListenerFactory,
    std::string fontFolderPath)
    : Image360(std::move(renderer), std::move(sceneFactory),
               std::move(modelFactory), std::move(diffuseTextureFactory),
               std::move(diffuseTextureCubeMapFactory),
               std::move(transformTreeFactory), std::move(cameraFactory),
               std::move(eventQueue), std::move(loggerFactory),
               std::move(uiFactory), std::move(gazeDetectorFactory),
               std::move(eventGazeListenerFactory), std::move(fontFolderPath)) {
  this->logger->setTag("Image360Stereo::");
}

Image360Stereo::~Image360Stereo() {}

void Image360Stereo::start() {
  if (!renderer->start()) {
    this->logger->log(LOG_INFO, "Application start failed");
    return;
  }
  this->logger->log(LOG_INFO, "Application started.");
}

void Image360Stereo::initialize(
    TEXTURE_MAP_MODE mapMode,
    std::vector<std::unique_ptr<Image>> &textureImages) {
  std::unique_ptr<Camera> camera;
  scene = sceneFactory->create("Image360:Stereo");
  assert(scene != nullptr);
  scene->setBackgroundColor(CL_Vec4(0.0f, 0.4f, 0.0f, 0.0f));
  scene->setDepthTest(true);
  scene->setBlending(true);
  camera = cameraFactory->create("camera", scene.get());
  assert(camera != nullptr);
  camera->setAspect(1.5f);
  camera->setFarPlane(1000.0f);
  camera->setFov(1.5f);
  camera->setNearPlane(1.0f);
  this->camera = camera.get();
  scene->addToScene(std::move(camera));

  std::unique_ptr<Model> imageContainerLeft =
      modelFactory->create("imageContainerLeft");
  assert(imageContainerLeft != nullptr);
  this->stereoImageContainer[0] = imageContainerLeft.get();
  scene->addToScene(std::move(imageContainerLeft));

  std::unique_ptr<Model> imageContainerRight =
      modelFactory->create("imageContainerRight");
  assert(imageContainerRight != nullptr);
  this->stereoImageContainer[1] = imageContainerRight.get();
  scene->addToScene(std::move(imageContainerRight));

  TransformTreeCamera *transformTreeCamera = nullptr;
  if (mapMode == EQUIRECTANGULAR_MAP_MODE) {
    std::unique_ptr<ShaderDiffuseTexture> shaderDiffuseTexture;
    std::unique_ptr<MaterialDiffuseTexture> materialDiffuseTexture;
    std::unique_ptr<Texture> imageTexture;

    shaderDiffuseTexture =
        diffuseTextureFactory->createShader("shader", scene.get());
    assert(shaderDiffuseTexture != nullptr);
    this->stereoShader = shaderDiffuseTexture.get();
    scene->addToScene(std::move(shaderDiffuseTexture));

    materialDiffuseTexture = diffuseTextureFactory->createMaterial(
        "material", (ShaderDiffuseTexture *)this->stereoShader);
    assert(materialDiffuseTexture != nullptr);
    this->stereoMaterial = materialDiffuseTexture.get();
    scene->addToScene(std::move(materialDiffuseTexture));

    assert(textureImages.size() == 1);
    assert(textureImages[0] != nullptr);

    // todo - change texture mapping of models to do this
    imageTexture = diffuseTextureFactory->createTexture("imageTexture");
    assert(imageTexture != nullptr);

    std::unique_ptr<Image> image = std::move(textureImages[0]);
    imageTexture->setTextureData(std::move(image->data));
    imageTexture->setHeight(image->height);
    imageTexture->setWidth(image->width);
    imageTexture->setTextureDataSize(image->dataSize);
    this->stereoImageTexture = imageTexture.get();
    scene->addToScene(std::move(imageTexture));
    ((MaterialDiffuseTexture *)this->stereoMaterial)
        ->setDiffuseTexture(this->stereoImageTexture);

    std::unique_ptr<TransformTreeCamera> transformTreeCameraUptr =
        transformTreeFactory->createTransformTreeCamera(this->camera);
    this->camera->getComponentList().addComponent(
        std::move(transformTreeCameraUptr));

    std::unique_ptr<TransformTreeModel> leftTransformSphereUptr =
        transformTreeFactory->createTransformTreeModel(
            this->stereoImageContainer[LEFT]);
    this->stereoImageContainer[LEFT]->getComponentList().addComponent(
        std::move(leftTransformSphereUptr));

    std::unique_ptr<TransformTreeModel> rightTransformSphereUptr =
        transformTreeFactory->createTransformTreeModel(
            this->stereoImageContainer[RIGHT]);
    this->stereoImageContainer[RIGHT]->getComponentList().addComponent(
        std::move(rightTransformSphereUptr));

    transformTreeCamera =
        (TransformTreeCamera *)this->camera->getComponentList().getComponent(
            "transformTree");
    transformTreeCamera->setLocalPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
    transformTreeCamera->setLocalRotation(CL_Vec3(0.0f, 0.0f, 0.0f));

    TransformTreeModel *transformSphereLeft =
        (TransformTreeModel *)this->stereoImageContainer[LEFT]
            ->getComponentList()
            .getComponent("transformTree");
    transformSphereLeft->setLocalPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
    transformSphereLeft->setLocalScale(CL_Vec3(100.0f, 100.0f, 100.0f));

    this->stereoImageContainer[LEFT]->createBiRelation(this->stereoMaterial);

    TransformTreeModel *transformSphereRight =
        (TransformTreeModel *)this->stereoImageContainer[RIGHT]
            ->getComponentList()
            .getComponent("transformTree");
    transformSphereRight->setLocalPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
    transformSphereRight->setLocalScale(CL_Vec3(100.0f, 100.0f, 100.0f));

    this->stereoImageContainer[RIGHT]->createBiRelation(this->stereoMaterial);

    std::unique_ptr<ModelModifier> modelModifierLeft(new ModelModifier);
    UVSphereBuilder uvSphereBuilderLeft(modelModifierLeft.get());
    uvSphereBuilderLeft.setVMax(0.5f);
    uvSphereBuilderLeft.buildUnitSphere(this->stereoImageContainer[LEFT], 5);

    std::unique_ptr<ModelModifier> modelModifierRight(new ModelModifier);
    UVSphereBuilder uvSphereBuilderRight(modelModifierRight.get());
    uvSphereBuilderRight.setVMin(0.5f);
    uvSphereBuilderRight.buildUnitSphere(this->stereoImageContainer[RIGHT], 5);
  } else {
    // todo - CubeMap format for stereo images
  }

  // Font Store Initialization
  std::unique_ptr<FontStore> fontStore =
      uiFactory->createFontStore(scene.get(), this->fontFolderPath.c_str());

  // Setting Text Styles
  TextStyle textStyle;
  textStyle.fontSize = 20;
  textStyle.scale = 0.025f;
  textStyle.color = CL_Vec4(1.0, 1.0, 1.0, 1.0);

  // Action Button Initializat
  auto actionButtonPosition = CL_Vec3(-5.1, 0.0, -15.5);
  auto actionButtonRotation = CL_Vec3(0.0, 0.0, 0.0);
  auto actionButtonColor = CL_Vec4(0.0, 0.0, 0.0, 0.7);
  auto actionButtonWidth = 3.0f;
  auto actionButtonHeight = 1.0f;
  this->actionButtonBackground = uiFactory->createPlanarBackground(
      "actionButton", scene.get(), actionButtonColor, actionButtonPosition,
      actionButtonRotation, actionButtonWidth, actionButtonHeight);

  // action button text position & orientation is relative to the actionButton
  // planar background
  auto actionTextPosition = CL_Vec3(-1.0, -0.2, 0.001);
  auto actionTextRotation = CL_Vec3(0.0, 0.0, 0.0);
  std::unique_ptr<TextElement> actionButtonElement =
      uiFactory->createTextElement("actionButtonElement", fontStore.get(),
                                   &textStyle, this->actionButtonText,
                                   actionTextPosition, actionTextRotation,
                                   scene.get());
  this->actionButtonBackground->addChild("child1",
                                         std::move(actionButtonElement));

  // Close Button Intialization
  auto closeButtonPosition = CL_Vec3(5.1, 0.0, -15.5);
  auto closeButtonRotation = CL_Vec3(0.0, 0.0, 0.0);
  auto closeButtonColor = CL_Vec4(0.0, 0.0, 0.0, 0.7);
  auto closeButtonWidth = 3.0f;
  auto closeButtonHeight = 1.0f;
  this->closeButtonBackground = uiFactory->createPlanarBackground(
      "closeButton", scene.get(), closeButtonColor, closeButtonPosition,
      closeButtonRotation, closeButtonWidth, closeButtonHeight);

  // close button text position & orientation is relative to the actionButton
  // planar background
  auto closeTextPosition = CL_Vec3(-1.0, -0.2, 1.0);
  auto closeTextRotation = CL_Vec3(0.0, 0.0, 0.0);
  std::unique_ptr<TextElement> closeButtonElement =
      uiFactory->createTextElement("closeButtonElement", fontStore.get(),
                                   &textStyle, this->closeButtonText,
                                   closeTextPosition, closeTextRotation,
                                   scene.get());
  this->closeButtonBackground->addChild("child2",
                                        std::move(closeButtonElement));

  gazeDetectorContainer = gazeDetectorFactory->createGazeDetectorContainer();

  // initializing action button model
  Model *actionButtonModel = (Model *)scene->getFromScene("actionButton");
  TransformTreeModel *transformActionButton =
      (TransformTreeModel *)actionButtonModel->getComponentList().getComponent(
          "transformTree");
  this->actionButtonListener =
      eventGazeListenerFactory->createActionButtonListener();

  Model *closeButtonModel = (Model *)scene->getFromScene("closeButton");
  TransformTreeModel *transformCloseButton =
      (TransformTreeModel *)closeButtonModel->getComponentList().getComponent(
          "transformTree");
  this->closeButtonListener =
      eventGazeListenerFactory->createCloseButtonListener();

  TransformTree *gazeTransformTarget = nullptr;
  auto reticleColor = CL_Vec4(0.0, 1.0, 0.0, 1.0);
  if (isControllerPresent) {
    reticleBase = uiFactory->createReticle("reticleBase", scene.get(), nullptr,
                                           reticleColor);
    TransformTreeModel *transform = reticleBase->getTransformTreeModel();
    reticle = uiFactory->createReticle("reticle", scene.get(), transform,
                                       reticleColor);
    gazeTransformTarget = transform;
  } else {
    reticle = uiFactory->createReticle("reticle", scene.get(),
                                       transformTreeCamera, reticleColor);
    gazeTransformTarget = transformTreeCamera;
  }

  auto actionButtonGazeDetectorOrigin = CL_Vec3(0.0f, 0.0f, 0.0f);
  auto actionButtonGazeDetectorLookAt = CL_Vec3(0.0f, 0.0f, -1.0f);
  auto actionButtonGazeDetectorLengthX = 3.0f;
  auto actionButtonGazeDetectorLengthY = 1.0f;
  auto actionButtonGazeDetectorLengthZ = 0.00001f;
  std::unique_ptr<IComponent> gazeDetectorNotifyMe =
      gazeDetectorFactory->createGazeDetectorBox(
          this->actionButtonText, gazeTransformTarget, transformActionButton,
          actionButtonListener.get(), gazeDetectorContainer.get(),
          actionButtonGazeDetectorOrigin, actionButtonGazeDetectorLookAt,
          actionButtonGazeDetectorLengthX, actionButtonGazeDetectorLengthY,
          actionButtonGazeDetectorLengthZ);
  actionButtonModel->getComponentList().addComponent(
      std::move(gazeDetectorNotifyMe));

  auto closeButtonGazeDetectorOrigin = CL_Vec3(0.0f, 0.0f, 0.0f);
  auto closeButtonGazeDetectorLookAt = CL_Vec3(0.0f, 0.0f, -1.0f);
  auto closeButtonGazeDetectorLengthX = 3.0f;
  auto closeButtonGazeDetectorLengthY = 1.0f;
  auto closeButtonGazeDetectorLengthZ = 0.00001f;
  std::unique_ptr<IComponent> gazeDetectorCloseMe =
      gazeDetectorFactory->createGazeDetectorBox(
          this->closeButtonText, gazeTransformTarget, transformCloseButton,
          closeButtonListener.get(), gazeDetectorContainer.get(),
          closeButtonGazeDetectorOrigin, closeButtonGazeDetectorLookAt,
          closeButtonGazeDetectorLengthX, closeButtonGazeDetectorLengthY,
          closeButtonGazeDetectorLengthZ);
  closeButtonModel->getComponentList().addComponent(
      std::move(gazeDetectorCloseMe));

  // fade screen initialization
  auto fadeScreenPosition = CL_Vec3(0.0, 0.0, -2.0);
  auto fadeScreenRotation = CL_Vec3(0.0, 0.0, 0.0);
  auto fadeScreenColor = CL_Vec4(0.0, 0.0, 0.0, 0.0);

  this->fadeScreen = uiFactory->createFadeScreen(
      "fadeScreen", scene.get(), fadeScreenColor, fadeScreenPosition,
      fadeScreenRotation, 20.0, 20.0);
  transformTreeCamera->addChild(this->fadeScreen->getTransformTree());

  renderer->initialize(scene.get());
  logger->log(LOG_DEBUG, "Initialization Completed!!");
}

void Image360Stereo::deinitialize() { renderer->deinitialize(scene.get()); }

void Image360Stereo::drawInit() {
  if (this->fadeStarted) {
    this->fadeScreen->setColor(CL_Vec4(0.0, 0.0, 0.0, this->alphaFade));
    if (this->alphaFade < 1.0) {
      this->alphaFade += this->fadeSpeed;
    } else {
      this->fadeComplete = true;
    }
  }

  while (!eventQueue->empty()) {
    std::unique_ptr<IEvent> event = eventQueue->pop();
    event->callListener();
  }
  // Render objects common to both eyes
  renderer->drawInit(scene.get());
}

void Image360Stereo::draw(EYE eye) {
  auto rightContainer =
      (Model *)this->scene->getFromScene("imageContainerRight");
  auto leftContainer = (Model *)this->scene->getFromScene("imageContainerLeft");

  if (eye == LEFT) {
    leftContainer->setIsVisible(true);
    rightContainer->setIsVisible(false);
  } else if (eye == RIGHT) {
    leftContainer->setIsVisible(false);
    rightContainer->setIsVisible(true);
  } else {
    return;
  }
  renderer->draw(scene.get(), eye);
}

void Image360Stereo::drawComplete() { renderer->drawComplete(); }

void Image360Stereo::update() { renderer->update(); }

void Image360Stereo::pause() { renderer->pause(); }

void Image360Stereo::resume() { renderer->resume(); }

void Image360Stereo::stop() { renderer->stop(); }

void Image360Stereo::onKeyPress(char key, int x, int y) {
  logger->log(LOG_DEBUG, "Key pressed:" + std::string(1, key));
  TransformTreeCamera *transform =
      (TransformTreeCamera *)camera->getComponentList().getComponent(
          "transformTree");
  CL_Vec3 pos = transform->getLocalPosition();

  if (key == 'W') {
    pos[2] -= 0.6f;
  } else if (key == 'S') {
    pos[2] += 0.6f;
  } else if (key == 'A') {
    pos[0] -= 0.6f;
  } else if (key == 'D') {
    pos[0] += 0.6f;
  } else {
    return;
  }
  transform->setLocalPosition(pos);
}

void Image360Stereo::onPassiveMouseMotion(int x, int y) {
  // logger->log(LOG_DEBUG, "Mouse move:" + std::to_string(x) + "," +
  // std::to_string(y));
  if (lastPassiveMousePositionX != -1) {
    float xoff =
        (x - lastPassiveMousePositionX) * passiveMouseMotionSensitivity;
    float yoff =
        (y - lastPassiveMousePositionY) * passiveMouseMotionSensitivity;

    TransformTreeCamera *transform =
        (TransformTreeCamera *)camera->getComponentList().getComponent(
            "transformTree");
    CL_Vec3 rotation = transform->getLocalRotation();
    transform->setLocalRotation(
        CL_Vec3(rotation.x - yoff, rotation.y - xoff, rotation.z));
  }
  lastPassiveMousePositionX = x;
  lastPassiveMousePositionY = y;
}

IRenderer *Image360Stereo::getRenderer() { return this->renderer.get(); }
}