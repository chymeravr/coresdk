#include <assert.h>
#include <coreEngine/components/transformTree/TransformTreeFactory.h>
#include <coreEngine/modelBuilder/CubeBuilder.h>
#include <coreEngine/modelBuilder/ModelLoader.h>
#include <coreEngine/modelBuilder/UVSphereBuilder.h>
#include <coreEngine/modifier/ImageModifier.h>
#include <coreEngine/modifier/ModelModifier.h>
#include <image360/Image360.h>

namespace cl {
void Image360::start() {
  this->renderer->start();
  this->logger->log(LOG_INFO, "Application started.");
}

void Image360::initialize() {
  std::unique_ptr<Camera> camera;

  scene = sceneFactory->create("testScene");
  assert(scene != nullptr);
  scene->setBackgroundColor(CL_Vec4(0.0f, 0.0f, 0.4f, 0.0f));
  camera = cameraFactory->create("camera", scene.get());
  assert(camera != nullptr);
  camera->setAspect(1.5f);
  camera->setFarPlane(1000.0f);
  camera->setFov(1.5f);
  camera->setNearPlane(1.0f);
  this->camera = camera.get();
  scene->addToScene(std::move(camera));

  // Font store Intialization
  this->fontStore =
      uiFactory->createFontStore(scene.get(), this->fontFolderPath.c_str());
}

void Image360::initMonoView() {
  // Initialize an empty model for holding the sphere for 360 image
  std::unique_ptr<Model> imageContainer =
      modelFactory->create("imageContainer");
  assert(imageContainer != nullptr);
  this->imageContainer = imageContainer.get();
  scene->addToScene(std::move(imageContainer));
}

void Image360::initMonoEquirectangularView(
    std::unique_ptr<Image> textureImage) {
  assert(imageTexture != nullptr);

  std::unique_ptr<ShaderDiffuseTexture> shaderDiffuseTexture;
  std::unique_ptr<MaterialDiffuseTexture> materialDiffuseTexture;
  std::unique_ptr<Texture> imageTexture;
  shaderDiffuseTexture =
      diffuseTextureFactory->createShader("shader", scene.get());
  assert(shaderDiffuseTexture != nullptr);
  this->shader = shaderDiffuseTexture.get();
  scene->addToScene(std::move(shaderDiffuseTexture));

  materialDiffuseTexture = diffuseTextureFactory->createMaterial(
      "material", (ShaderDiffuseTexture *)this->shader);
  assert(materialDiffuseTexture != nullptr);
  this->material = materialDiffuseTexture.get();
  scene->addToScene(std::move(materialDiffuseTexture));

  imageTexture = diffuseTextureFactory->createTexture("imageTexture");

  std::unique_ptr<Image> image = std::move(textureImage);
  imageTexture->setTextureData(std::move(image->data));
  imageTexture->setHeight(image->height);
  imageTexture->setWidth(image->width);
  imageTexture->setTextureDataSize(image->dataSize);
  this->imageTexture = imageTexture.get();
  scene->addToScene(std::move(imageTexture));

  ((MaterialDiffuseTexture *)this->material)
      ->setDiffuseTexture(this->imageTexture);
  std::unique_ptr<TransformTreeCamera> transformTreeCameraUptr =
      transformTreeFactory->createTransformTreeCamera(this->camera);
  this->camera->getComponentList().addComponent(
      std::move(transformTreeCameraUptr));

  std::unique_ptr<TransformTreeModel> transformSphereUptr =
      transformTreeFactory->createTransformTreeModel(this->imageContainer);
  this->imageContainer->getComponentList().addComponent(
      std::move(transformSphereUptr));

  TransformTreeCamera *transformTreeCamera =
      (TransformTreeCamera *)this->camera->getComponentList().getComponent(
          "transformTree");
  transformTreeCamera->setLocalPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
  transformTreeCamera->setLocalRotation(CL_Vec3(0.0f, 0.0f, 0.0f));

  TransformTreeModel *transformSphere =
      (TransformTreeModel *)this->imageContainer->getComponentList()
          .getComponent("transformTree");
  transformSphere->setLocalPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
  transformSphere->setLocalScale(CL_Vec3(100.0f, 100.0f, 100.0f));

  this->imageContainer->createBiRelation(this->material);

  std::unique_ptr<ModelModifier> modelModifier(new ModelModifier);
  UVSphereBuilder uvSphereBuilder(modelModifier.get());
  uvSphereBuilder.buildUnitSphere(this->imageContainer, 5);
}

void Image360::initMonoCubeMapSingleTextureView(
    std::unique_ptr<Image> textureImage) {
  std::unique_ptr<ShaderDiffuseTextureCubeMap> shaderUPtr =
      diffuseTextureCubeMapFactory->createShader("shader", scene.get());
  assert(shaderUPtr != nullptr);
  this->shader = shaderUPtr.get();
  scene->addToScene(std::move(shaderUPtr));

  std::unique_ptr<MaterialDiffuseTextureCubeMap> materialUPtr =
      diffuseTextureCubeMapFactory->createMaterial(
          "material", (ShaderDiffuseTextureCubeMap *)this->shader);
  assert(materialUPtr != nullptr);
  this->material = materialUPtr.get();
  scene->addToScene(std::move(materialUPtr));

  std::unique_ptr<Image> frontImage(new Image);
  std::unique_ptr<Image> leftImage(new Image);
  std::unique_ptr<Image> backImage(new Image);
  std::unique_ptr<Image> rightImage(new Image);
  std::unique_ptr<Image> topImage(new Image);
  std::unique_ptr<Image> bottomImage(new Image);

  //   assert(textureImages.size() == 1);
  //   assert(textureImages[0] != nullptr);
  assert(textureImage != nullptr);
  std::unique_ptr<Image> originalImage = std::move(textureImage);
  ImageModifier imageModifier(logger.get());
  imageModifier.convertSingleCubicImageIntoSix(
      originalImage.get(), frontImage.get(), leftImage.get(), backImage.get(),
      rightImage.get(), bottomImage.get(), topImage.get());

  assert(rightImage != nullptr && leftImage != nullptr && topImage != nullptr &&
         bottomImage != nullptr && backImage != nullptr &&
         frontImage != nullptr);

  initCubeMapTexture(rightImage.get(), leftImage.get(), topImage.get(),
                     bottomImage.get(), frontImage.get(), backImage.get());
}

void Image360::initCubeMapTexture(Image *rightImage, Image *leftImage,
                                  Image *topImage, Image *bottomImage,
                                  Image *frontImage, Image *backImage) {
  std::unique_ptr<TextureCubeMap> textureUPtr =
      diffuseTextureCubeMapFactory->createTexture("imageTexture");
  assert(textureUPtr != nullptr);

  // using an array of images initialized in same order as the texture enum to
  // enable looping
  Image *cubeMapImages[6] = {leftImage, rightImage, frontImage,
                             backImage, topImage,   bottomImage};
  for (int i = 0; i < 6; i++) {
    TEXTURE_CUBE_MAP_FACE cubeMapTexture =
        static_cast<TEXTURE_CUBE_MAP_FACE>(i);

    textureUPtr->setTextureData(cubeMapTexture,
                                std::move(cubeMapImages[i]->data));
    textureUPtr->setHeight(cubeMapTexture, cubeMapImages[i]->height);
    textureUPtr->setWidth(cubeMapTexture, cubeMapImages[i]->width);
    textureUPtr->setTextureDataSize(cubeMapTexture, cubeMapImages[i]->dataSize);
  }

  this->imageTexture = textureUPtr.get();
  scene->addToScene(std::move(textureUPtr));

  ((MaterialDiffuseTextureCubeMap *)this->material)
      ->setDiffuseTexture(this->imageTexture);

  std::unique_ptr<TransformTreeCamera> transformTreeCameraUptr =
      transformTreeFactory->createTransformTreeCamera(this->camera);
  this->camera->getComponentList().addComponent(
      std::move(transformTreeCameraUptr));

  std::unique_ptr<TransformTreeModel> transformSphereUptr =
      transformTreeFactory->createTransformTreeModel(this->imageContainer);
  this->imageContainer->getComponentList().addComponent(
      std::move(transformSphereUptr));

  TransformTreeCamera *transformTreeCamera =
      (TransformTreeCamera *)this->camera->getComponentList().getComponent(
          "transformTree");
  transformTreeCamera->setLocalPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
  transformTreeCamera->setLocalRotation(CL_Vec3(0.0f, 0.0f, 0.0f));

  TransformTreeModel *transformSphere =
      (TransformTreeModel *)this->imageContainer->getComponentList()
          .getComponent("transformTree");
  transformSphere->setLocalPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
  transformSphere->setLocalScale(CL_Vec3(100.0f, 100.0f, 100.0f));

  this->imageContainer->createBiRelation(this->material);

  std::unique_ptr<ModelModifier> modelModifier(new ModelModifier);
  CubeBuilder cubeBuilder(modelModifier.get());
  cubeBuilder.buildInwardCube(this->imageContainer);
}

void Image360::initStereoView() {
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
}

void Image360::initStereoEquirectangularView(
    std::unique_ptr<Image> textureImage) {
  assert(textureImage != nullptr);

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

  // todo - change texture mapping of models to do this
  imageTexture = diffuseTextureFactory->createTexture("imageTexture");

  std::unique_ptr<Image> image = std::move(textureImage);
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

  TransformTreeCamera *transformTreeCamera =
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

  this->stereoImageContainer[0]->setDepthTest(true);
  this->stereoImageContainer[1]->setDepthTest(true);
  this->stereoImageContainer[0]->setBlending(true);
  this->stereoImageContainer[1]->setBlending(true);
}

void Image360::initUIButtons() {
  // Text style info
  TextStyle textStyle;
  textStyle.fontSize = 20;
  textStyle.scale = 0.025f;
  textStyle.color = CL_Vec4(1.0, 1.0, 1.0, 1.0);

  // Laser Box Initialization
  auto laserPosition = CL_Vec3(-5.1, 5.0, -15.5);
  auto laserRotation = CL_Vec3(0.0, 0.0, 0.0);
  auto laserColor = CL_Vec4(1.0, 1.0, 1.0, 0.7);
  auto laserWidth = 0.1f;
  auto laserHeight = 10.0f;
  this->laserBox = uiFactory->createPlanarBackground(
      "laserBox", scene.get(), laserColor, laserPosition, laserRotation,
      laserWidth, laserHeight);

  // Action Button Initialization
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
  auto closeTextPosition = CL_Vec3(-0.5, -0.2, 0.001);
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

  // initializing close button model
  Model *closeButtonModel = (Model *)scene->getFromScene("closeButton");
  TransformTreeModel *transformCloseButton =
      (TransformTreeModel *)closeButtonModel->getComponentList().getComponent(
          "transformTree");
  this->closeButtonListener =
      eventGazeListenerFactory->createCloseButtonListener();

  // gaze detectors for action and close button
  auto actionButtonGazeDetectorOrigin = CL_Vec3(0.0f, 0.0f, 0.0f);
  auto actionButtonGazeDetectorLookAt = CL_Vec3(0.0f, 0.0f, -1.0f);
  auto actionButtonGazeDetectorLengthX = 3.0f;
  auto actionButtonGazeDetectorLengthY = 1.0f;
  auto actionButtonGazeDetectorLengthZ = 0.00001f;
  std::unique_ptr<IComponent> actionButtonGazeDetector =
      gazeDetectorFactory->createGazeDetectorBox(
          this->actionButtonText, gazeTransformTarget, transformActionButton,
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
      gazeDetectorFactory->createGazeDetectorBox(
          this->closeButtonText, gazeTransformTarget, transformCloseButton,
          closeButtonListener.get(), gazeDetectorContainer.get(),
          closeButtonGazeDetectorOrigin, closeButtonGazeDetectorLookAt,
          closeButtonGazeDetectorLengthX, closeButtonGazeDetectorLengthY,
          closeButtonGazeDetectorLengthZ);
  closeButtonModel->getComponentList().addComponent(
      std::move(closeButtonGazeDetector));
}

void Image360::initCameraReticle() {
  // create a reticle attached to the main camera
  auto reticleColor = CL_Vec4(0.0, 1.0, 0.0, 1.0);

  TransformTreeCamera *transformTreeCamera =
      (TransformTreeCamera *)this->camera->getComponentList().getComponent(
          "transformTree");
  reticle = this->uiFactory->createReticle("reticle", scene.get(),
                                           transformTreeCamera, reticleColor);
  this->gazeTransformTarget = transformTreeCamera;
}

void Image360::initControllerReticle() {
  // create a reticle attached to the controller
  auto reticleColor = CL_Vec4(0.0, 1.0, 0.0, 1.0);
  reticleBase = uiFactory->createReticle("reticleBase", scene.get(), nullptr,
                                         reticleColor);
  TransformTreeModel *transform = reticleBase->getTransformTreeModel();
  reticle =
      uiFactory->createReticle("reticle", scene.get(), transform, reticleColor);
  this->gazeTransformTarget = transform;
}

void Image360::initController(std::unique_ptr<Image> controllerImage,
                              std::string controllerModelPath) {
  // todo :
  // 1. create object model
  // 2. initialize diffuse shader
  // 3. map shader to model texture
  std::unique_ptr<Model> controllerModelContainer =
      modelFactory->create("controllerModel");
  assert(controllerModelContainer != nullptr);
  this->controllerModel = controllerModelContainer.get();
  this->scene->addToScene(std::move(controllerModelContainer));
  std::unique_ptr<ModelLoader> modelLoader =
      std::unique_ptr<ModelLoader>(new ModelLoader(this->loggerFactory));
  modelLoader->load_obj(controllerModelPath, this->controllerModel);
  this->controllerModel->setDepthTest(true);
  this->controllerModel->setBlending(true);

  std::unique_ptr<TransformTreeModel> controllerTransformUptr =
      this->transformTreeFactory->createTransformTreeModel(
          this->controllerModel);
  this->controllerModel->getComponentList().addComponent(
      std::move(controllerTransformUptr));

  TransformTreeModel *transformController =
      (TransformTreeModel *)this->controllerModel->getComponentList()
          .getComponent("transformTree");
  transformController->setLocalPosition(CL_Vec3(0.0f, 0.0f, -5.0f));
  transformController->setLocalScale(CL_Vec3(100.0f, 100.0f, 100.0f));
  transformController->setLocalRotation(CL_Vec3(90.0f, 90.0f, 90.0f));

  std::unique_ptr<ShaderDiffuseTexture> controllerShaderUptr;
  std::unique_ptr<MaterialDiffuseTexture> controllerMaterialUptr;
  std::unique_ptr<Texture> controllerTextureUptr;

  // doubt ~ why is this named shader for 360 degree spheres
  controllerShaderUptr = this->diffuseTextureFactory->createShader(
      "controllerShader", scene.get());
  assert(controllerShaderUptr != nullptr);
  this->controllerShader = controllerShaderUptr.get();
  controllerShaderUptr->enableAlphaChannel();
  this->scene->addToScene(std::move(controllerShaderUptr));

  controllerMaterialUptr = this->diffuseTextureFactory->createMaterial(
      "controllerMaterial", (ShaderDiffuseTexture *)this->controllerShader);
  assert(controllerMaterialUptr != nullptr);
  this->controllerMaterial = controllerMaterialUptr.get();
  this->scene->addToScene(std::move(controllerMaterialUptr));

  controllerTextureUptr =
      this->diffuseTextureFactory->createTexture("controllerTexture");

  std::unique_ptr<Image> controllerImageUptr = std::move(controllerImage);
  controllerTextureUptr->setTextureData(std::move(controllerImageUptr->data));
  controllerTextureUptr->setHeight(controllerImageUptr->height);
  controllerTextureUptr->setWidth(controllerImageUptr->width);
  controllerTextureUptr->setTextureDataSize(controllerImageUptr->dataSize);
  controllerTextureUptr->setColorFormat(Texture::ColorFormat::RGBA);

  this->controllerTexture = controllerTextureUptr.get();
  this->scene->addToScene(std::move(controllerTextureUptr));
  ((MaterialDiffuseTexture *)this->controllerMaterial)
      ->setDiffuseTexture(this->controllerTexture);

  this->controllerModel->createBiRelation(this->controllerMaterial);
  return;
}

void Image360::initFadeScreen() {
  // fade screen initialization
  auto fadeScreenPosition = CL_Vec3(0.0, 0.0, -2.0);
  auto fadeScreenRotation = CL_Vec3(0.0, 0.0, 0.0);
  auto fadeScreenColor =
      CL_Vec4(0.0, 0.0, 0.0, 0.0);  // change to 1.0 for start

  this->fadeScreen = uiFactory->createFadeScreen(
      "fadeScreen", scene.get(), fadeScreenColor, fadeScreenPosition,
      fadeScreenRotation, 20.0, 20.0);
  TransformTreeCamera *transformTreeCamera =
      (TransformTreeCamera *)this->camera->getComponentList().getComponent(
          "transformTree");
  transformTreeCamera->addChild(this->fadeScreen->getTransformTree());
}

void Image360::initComplete() { this->renderer->initialize(scene.get()); }

void Image360::update() { renderer->update(); }

void Image360::drawInit() {
  if (this->fadeStarted) {
    if (this->fadeScreen != nullptr) {
      this->fadeScreen->setColor(CL_Vec4(0.0, 0.0, 0.0, this->alphaFade));
      if (this->alphaFade < 1.0) {
        this->alphaFade += this->fadeSpeed;
      } else {
        this->fadeComplete = true;
      }
    } else {
      this->fadeComplete = true;
    }
  } else if (this->alphaFade > 0.0f && this->alphaFade <= 1.0f) {
    if (this->fadeScreen != nullptr) {
      this->fadeScreen->setColor(CL_Vec4(0.0, 0.0, 0.0, this->alphaFade));
    }
    this->alphaFade -= this->fadeSpeed / 1.5;
  }

  while (!eventQueue->empty()) {
    std::unique_ptr<IEvent> event = eventQueue->pop();
    event->callListener();
  }
  renderer->drawInit(scene.get());
}

// void Image360::draw(EYE eye) { renderer->draw(scene.get(), eye); }
void Image360::drawMono() {
  renderer->draw(scene.get(), LEFT);
  renderer->draw(scene.get(), RIGHT);
}

void Image360::drawStereoLeft() {
  stereoImageContainer[LEFT]->setIsVisible(true);
  stereoImageContainer[RIGHT]->setIsVisible(false);
  renderer->draw(scene.get(), LEFT);
}

void Image360::drawStereoRight() {
  stereoImageContainer[LEFT]->setIsVisible(false);
  stereoImageContainer[RIGHT]->setIsVisible(true);
  renderer->draw(scene.get(), RIGHT);
}

void Image360::drawComplete() { renderer->drawComplete(); }

void Image360::deinitialize() { renderer->deinitialize(scene.get()); }
void Image360::stop() { renderer->stop(); }

void Image360::pause() { renderer->pause(); }

void Image360::resume() { renderer->resume(); }

void Image360::onKeyPress(char key, int x, int y) {
  // logger->log(LOG_DEBUG, "Key pressed:" + std::string(1, key));
  logger->log(LOG_DEBUG, "Key pressed:" + std::string(1, key));
  TransformTreeModel *transform = reticle->getTransformTreeModel();
  CL_Vec3 loc = transform->getParent()->getLocalPosition();

  if (key == 'W') {
    loc[2] -= 0.2f;
  } else if (key == 'S') {
    loc[2] += 0.2f;
  } else if (key == 'A') {
    loc[0] -= 0.2f;
  } else if (key == 'D') {
    loc[0] += 0.2f;
  } else {
    return;
  }
  transform->getParent()->setLocalPosition(loc);
}

void Image360::onPassiveMouseMotion(int x, int y) {
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

IRenderer *Image360::getRenderer() { return this->renderer.get(); }
}