#include <assert.h>
#include <coreEngine/components/transformTree/TransformTreeFactory.h>
#include <coreEngine/modelBuilder/CubeBuilder.h>
#include <coreEngine/modelBuilder/ModelLoader.h>
#include <coreEngine/modelBuilder/RectangleBuilder.h>
#include <coreEngine/modelBuilder/UVSphereBuilder.h>
#include <coreEngine/modifier/ImageModifier.h>
#include <coreEngine/modifier/ModelModifier.h>
#include <image360/Image360.h>
#include <image360/Image360EventKeyPressListener.h>
#include <image360/Image360EventPassiveMouseMotionListener.h>

namespace cl {
Image360::Image360(
    std::unique_ptr<IRenderer> renderer,
    std::unique_ptr<ISceneFactory> sceneFactory,
    std::unique_ptr<IModelFactory> modelFactory,
    std::unique_ptr<IDiffuseTextureFactory> diffuseTextureFactory,
    std::unique_ptr<IDiffuseTextureCubeMapFactory> diffuseTextureCubeMapFactory,
    std::unique_ptr<ITransformTreeFactory> transformTreeFactory,
    std::unique_ptr<ICameraFactory> cameraFactory, IEventQueue *eventQueue,
    ILoggerFactory *loggerFactory, std::unique_ptr<UIFactory> uiFactory,
    std::unique_ptr<GazeDetectorFactory> gazeDetectorFactory,
    std::unique_ptr<IEventGazeListenerFactory> gazeEventListenerFactory,
    std::string fontFolderPath) {
  assert(renderer != nullptr);
  assert(sceneFactory != nullptr);
  assert(modelFactory != nullptr);
  assert(diffuseTextureFactory != nullptr);
  assert(diffuseTextureCubeMapFactory != nullptr);
  assert(transformTreeFactory != nullptr);
  assert(eventQueue != nullptr);
  assert(cameraFactory != nullptr);
  assert(uiFactory != nullptr);
  assert(gazeDetectorFactory != nullptr);
  assert(gazeEventListenerFactory != nullptr);

  this->renderer = std::move(renderer);
  this->sceneFactory = std::move(sceneFactory);
  this->modelFactory = std::move(modelFactory);
  this->diffuseTextureFactory = std::move(diffuseTextureFactory);
  this->diffuseTextureCubeMapFactory = std::move(diffuseTextureCubeMapFactory);
  this->transformTreeFactory = std::move(transformTreeFactory);
  this->cameraFactory = std::move(cameraFactory);
  this->eventQueue = eventQueue;
  this->logger = loggerFactory->createLogger("Image360::");
  this->uiFactory = std::move(uiFactory);
  this->gazeDetectorFactory = std::move(gazeDetectorFactory);
  this->eventGazeListenerFactory = std::move(gazeEventListenerFactory);
  this->fontFolderPath = fontFolderPath;

  this->loggerFactory = loggerFactory;
}

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
  camera->setFov(1.75f);
  camera->setNearPlane(0.1f);
  this->camera = camera.get();
  scene->addToScene(std::move(camera));

  // Font store Intialization
  this->fontStore =
      uiFactory->createFontStore(scene.get(), this->fontFolderPath.c_str());

  this->eventKeyPressListener = std::unique_ptr<EventKeyPressListener>(
      new Image360EventKeyPressListener(this, this->loggerFactory));
  this->eventPassiveMouseMotionListener =
      std::unique_ptr<EventPassiveMouseMotionListener>(
          new Image360EventPassiveMouseMotionListener(this,
                                                      this->loggerFactory));
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
          this->stereoImageContainer[Image360::EYE::LEFT]);
  this->stereoImageContainer[Image360::EYE::LEFT]
      ->getComponentList()
      .addComponent(std::move(leftTransformSphereUptr));

  std::unique_ptr<TransformTreeModel> rightTransformSphereUptr =
      transformTreeFactory->createTransformTreeModel(
          this->stereoImageContainer[Image360::EYE::RIGHT]);
  this->stereoImageContainer[Image360::EYE::RIGHT]
      ->getComponentList()
      .addComponent(std::move(rightTransformSphereUptr));

  TransformTreeCamera *transformTreeCamera =
      (TransformTreeCamera *)this->camera->getComponentList().getComponent(
          "transformTree");
  transformTreeCamera->setLocalPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
  transformTreeCamera->setLocalRotation(CL_Vec3(0.0f, 0.0f, 0.0f));

  TransformTreeModel *transformSphereLeft =
      (TransformTreeModel *)this->stereoImageContainer[Image360::EYE::LEFT]
          ->getComponentList()
          .getComponent("transformTree");
  transformSphereLeft->setLocalPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
  transformSphereLeft->setLocalScale(CL_Vec3(100.0f, 100.0f, 100.0f));

  this->stereoImageContainer[Image360::EYE::LEFT]->createBiRelation(
      this->stereoMaterial);

  TransformTreeModel *transformSphereRight =
      (TransformTreeModel *)this->stereoImageContainer[Image360::EYE::RIGHT]
          ->getComponentList()
          .getComponent("transformTree");
  transformSphereRight->setLocalPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
  transformSphereRight->setLocalScale(CL_Vec3(100.0f, 100.0f, 100.0f));

  this->stereoImageContainer[Image360::EYE::RIGHT]->createBiRelation(
      this->stereoMaterial);

  std::unique_ptr<ModelModifier> modelModifierLeft(new ModelModifier);
  UVSphereBuilder uvSphereBuilderLeft(modelModifierLeft.get());
  uvSphereBuilderLeft.setVMax(0.5f);
  uvSphereBuilderLeft.buildUnitSphere(
      this->stereoImageContainer[Image360::EYE::LEFT], 5);

  std::unique_ptr<ModelModifier> modelModifierRight(new ModelModifier);
  UVSphereBuilder uvSphereBuilderRight(modelModifierRight.get());
  uvSphereBuilderRight.setVMin(0.5f);
  uvSphereBuilderRight.buildUnitSphere(
      this->stereoImageContainer[Image360::EYE::RIGHT], 5);

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
  this->reticle = this->uiFactory->createReticle(
      "reticle", scene.get(), transformTreeCamera, reticleColor);
  this->gazeTransformTarget = transformTreeCamera;
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
  this->controllerModel->setBackFaceCulling(true);

  std::unique_ptr<TransformTreeModel> controllerTransformUptr =
      this->transformTreeFactory->createTransformTreeModel(
          this->controllerModel);
  this->controllerModel->getComponentList().addComponent(
      std::move(controllerTransformUptr));

  TransformTreeModel *transformController =
      (TransformTreeModel *)this->controllerModel->getComponentList()
          .getComponent("transformTree");

  transformController->setLocalPosition(CL_Vec3(0.65f, -0.65f, -1.0f));
  transformController->setLocalScale(CL_Vec3(25.0f, 25.0f, 25.0f));
  transformController->setLocalRotation(CL_Vec3(30.0f, 0.0f, 30.0f));

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
  //   controllerTextureUptr->setColorFormat(Texture::ColorFormat::RGB);

  this->controllerTexture = controllerTextureUptr.get();
  this->scene->addToScene(std::move(controllerTextureUptr));
  ((MaterialDiffuseTexture *)this->controllerMaterial)
      ->setDiffuseTexture(this->controllerTexture);

  this->controllerModel->createBiRelation(this->controllerMaterial);
  return;
}

void Image360::initControllerLaser(std::unique_ptr<Image> laserBeamImage) {
  std::unique_ptr<Model> laserBeamModelUptr = modelFactory->create("laserBeam");
  assert(laserBeamModelUptr != nullptr);
  this->laserBeamModel = laserBeamModelUptr.get();
  this->scene->addToScene(std::move(laserBeamModelUptr));
  std::unique_ptr<RectangleBuilder> rectangleBuilder =
      std::unique_ptr<RectangleBuilder>(
          new RectangleBuilder(this->loggerFactory));
  rectangleBuilder->buildRectangle(this->laserBeamModel, 0.01f, 2.5f);
  this->laserBeamModel->setDepthTest(true);
  this->laserBeamModel->setBlending(true);

  std::unique_ptr<TransformTreeModel> laserBeamTransformUptr =
      this->transformTreeFactory->createTransformTreeModel(
          this->laserBeamModel);
  this->laserBeamModel->getComponentList().addComponent(
      std::move(laserBeamTransformUptr));

  TransformTreeModel *transformController =
      (TransformTreeModel *)this->controllerModel->getComponentList()
          .getComponent("transformTree");

  TransformTreeModel *transformLaserBeam =
      (TransformTreeModel *)this->laserBeamModel->getComponentList()
          .getComponent("transformTree");

  transformController->addChild(transformLaserBeam);
  transformLaserBeam->setLocalPosition(CL_Vec3(0.0f, 0.0f, -0.125f));
  transformLaserBeam->setLocalScale(CL_Vec3(0.025, 0.1f, 0.1f));
  transformLaserBeam->setLocalRotation(CL_Vec3(270.0f, 0.0f, 0.0f));

  std::unique_ptr<ShaderDiffuseTexture> laserBeamShaderUptr;
  std::unique_ptr<MaterialDiffuseTexture> laserBeamMaterialUptr;
  std::unique_ptr<Texture> laserBeamTextureUptr;

  // doubt ~ why is this named shader for 360 degree spheres
  laserBeamShaderUptr =
      this->diffuseTextureFactory->createShader("laserBeamShader", scene.get());
  assert(laserBeamShaderUptr != nullptr);
  this->laserBeamShader = laserBeamShaderUptr.get();
  laserBeamShaderUptr->enableAlphaChannel();
  this->scene->addToScene(std::move(laserBeamShaderUptr));

  laserBeamMaterialUptr = this->diffuseTextureFactory->createMaterial(
      "laserBeamMaterial", (ShaderDiffuseTexture *)this->laserBeamShader);
  assert(laserBeamMaterialUptr != nullptr);
  this->laserBeamMaterial = laserBeamMaterialUptr.get();
  this->scene->addToScene(std::move(laserBeamMaterialUptr));

  laserBeamTextureUptr =
      this->diffuseTextureFactory->createTexture("laserBeamTexture");

  std::unique_ptr<Image> laserBeamImageUptr = std::move(laserBeamImage);
  laserBeamTextureUptr->setTextureData(std::move(laserBeamImageUptr->data));
  laserBeamTextureUptr->setHeight(laserBeamImageUptr->height);
  laserBeamTextureUptr->setWidth(laserBeamImageUptr->width);
  laserBeamTextureUptr->setTextureDataSize(laserBeamImageUptr->dataSize);
  laserBeamTextureUptr->setColorFormat(Texture::ColorFormat::RGBA);

  this->laserBeamTexture = laserBeamTextureUptr.get();
  this->scene->addToScene(std::move(laserBeamTextureUptr));
  ((MaterialDiffuseTexture *)this->laserBeamMaterial)
      ->setDiffuseTexture(this->laserBeamTexture);

  this->laserBeamModel->createBiRelation(this->laserBeamMaterial);
}

void Image360::initControllerReticle() {
  // create a reticle attached to the controller
  auto reticleColor = CL_Vec4(1.0f, 1.0f, 1.0f, 1.0f);
  TransformTreeModel *transformController =
      (TransformTreeModel *)this->controllerModel->getComponentList()
          .getComponent("transformTree");
  this->controllerReticle = uiFactory->createReticle(
      "controllerReticle", scene.get(), transformController, reticleColor);
  TransformTreeModel *transformReticle =
      this->controllerReticle->getTransformTreeModel();
  transformReticle->setLocalPosition(CL_Vec3(0.0f, 0.0f, -0.35f));
  transformReticle->setLocalScale(CL_Vec3(0.03f, 0.03f, 0.02f));
  this->gazeTransformTarget = transformController;
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
  renderer->drawLeft(scene.get());
  renderer->drawRight(scene.get());
}

void Image360::drawStereoLeft() {
  stereoImageContainer[Image360::EYE::LEFT]->setIsVisible(true);
  stereoImageContainer[Image360::EYE::RIGHT]->setIsVisible(false);
  renderer->drawLeft(scene.get());
}

void Image360::drawStereoRight() {
  stereoImageContainer[Image360::EYE::LEFT]->setIsVisible(false);
  stereoImageContainer[Image360::EYE::RIGHT]->setIsVisible(true);
  renderer->drawRight(scene.get());
}

void Image360::drawComplete() { renderer->drawComplete(); }

void Image360::deinitialize() { renderer->deinitialize(scene.get()); }
void Image360::stop() { renderer->stop(); }

void Image360::pause() { renderer->pause(); }

void Image360::resume() { renderer->resume(); }

Reticle *Image360::getParentReticle() {
  if (this->controllerReticle != nullptr) {
    return this->controllerReticle.get();
  }
  return this->reticleBase.get();
}

IRenderer *Image360::getRenderer() { return this->renderer.get(); }

void Image360::updateControllerQuaternion(CL_Quat controllerOrientation) {
  TransformTreeModel *transformController =
      (TransformTreeModel *)this->controllerModel->getComponentList()
          .getComponent("transformTree");
  transformController->setLocalQuaternion(controllerOrientation);
}
void Image360::updateControllerRotation(CL_Vec3 controllerOrientation) {
  TransformTreeModel *transformController =
      (TransformTreeModel *)this->controllerModel->getComponentList()
          .getComponent("transformTree");
  transformController->setLocalRotation(controllerOrientation);
}
void Image360::updateControllerPosition(CL_Vec3 controllerPosition) {
  TransformTreeModel *transformController =
      (TransformTreeModel *)this->controllerModel->getComponentList()
          .getComponent("transformTree");
  transformController->setLocalPosition(controllerPosition);
}
}