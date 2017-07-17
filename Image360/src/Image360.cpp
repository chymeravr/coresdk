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

  // camera transforms
  std::unique_ptr<TransformTreeCamera> transformTreeCameraUptr =
      transformTreeFactory->createTransformTreeCamera(this->camera);
  this->camera->getComponentList().addComponent(
      std::move(transformTreeCameraUptr));

  TransformTreeCamera *transformTreeCamera =
      (TransformTreeCamera *)this->camera->getComponentList().getComponent(
          "transformTree");
  transformTreeCamera->setLocalPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
  transformTreeCamera->setLocalRotation(CL_Vec3(0.0f, 0.0f, 0.0f));

  // Font store Intialization
  this->fontStore =
      uiFactory->createFontStore(scene.get(), this->fontFolderPath.c_str());

  this->stereoSphere = std::unique_ptr<StereoSphere>(new StereoSphere(
      this->loggerFactory, this->modelFactory.get(),
      this->diffuseTextureFactory.get(), this->transformTreeFactory.get()));

  this->monoSphere = std::unique_ptr<MonoSphere>(new MonoSphere(
      this->loggerFactory, this->modelFactory.get(),
      this->diffuseTextureFactory.get(), this->transformTreeFactory.get()));

  this->monoCubeMap = std::unique_ptr<MonoCubeMap>(
      new MonoCubeMap(this->loggerFactory, this->modelFactory.get(),
                      this->diffuseTextureCubeMapFactory.get(),
                      this->transformTreeFactory.get()));

  this->buttons = std::unique_ptr<Buttons>(
      new Buttons(*this->loggerFactory, *this->uiFactory.get(),
                  *this->gazeDetectorFactory.get(), *this->fontStore.get(),
                  *this->eventGazeListenerFactory.get()));
}

void Image360::initMonoEquirectangularView(
    std::unique_ptr<Image> textureImage) {
  assert(textureImage != nullptr);
  // iniitialize the mono sphere to project the equirectangular view
  this->monoSphere->initialize(scene.get(), std::move(textureImage));
}

void Image360::initMonoCubeMapSingleTextureView(
    std::unique_ptr<Image> textureImage) {
  assert(textureImage != nullptr);
  // initialize mono cube map to project the cubemap image
  this->monoCubeMap->initialize(scene.get(), std::move(textureImage));
}

void Image360::initStereoEquirectangularView(
    std::unique_ptr<Image> textureImage) {
  assert(textureImage != nullptr);
  // initialize stereo sphere to project the equirectangular stereo image
  this->stereoSphere->initialize(this->scene.get(), std::move(textureImage));
}

void Image360::initUIButtons() {
  // initialize UI buttons
  this->buttons->initialize(*this->scene, *this->gazeTransformTarget);
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

void Image360::drawMono() {
  renderer->drawLeft(scene.get());
  renderer->drawRight(scene.get());
}

void Image360::drawStereoLeft() {
  this->stereoSphere->drawLeft();
  renderer->drawLeft(scene.get());
}

void Image360::drawStereoRight() {
  this->stereoSphere->drawRight();
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