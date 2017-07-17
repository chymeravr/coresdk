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

  scene = sceneFactory->create("Image360Scene");
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

  this->controller = std::unique_ptr<Controller>(new Controller(
      *this->loggerFactory, *this->modelFactory.get(),
      *this->transformTreeFactory.get(), *this->diffuseTextureFactory.get(),
      *this->uiFactory.get()));
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
  this->controller->initializeController(*scene, std::move(controllerImage),
                                         controllerModelPath);
}

void Image360::initControllerLaser(std::unique_ptr<Image> laserBeamImage) {
  this->controller->initializeControllerLaser(*scene,
                                              std::move(laserBeamImage));
}

void Image360::initControllerReticle() {
  this->controller->initializeControllerReticle(*scene);
  this->gazeTransformTarget = this->controller->getGazeTransformTarget();
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
  if (this->controller->getControllerReticle() != nullptr) {
    return this->controller->getControllerReticle();
  }
  return this->reticleBase.get();
}

IRenderer *Image360::getRenderer() { return this->renderer.get(); }

void Image360::updateControllerQuaternion(CL_Quat controllerOrientation) {
  TransformTreeModel *transformController =
      (TransformTreeModel *)this->controller->getControllerModel()
          ->getComponentList()
          .getComponent("transformTree");
  transformController->setLocalQuaternion(controllerOrientation);
}
void Image360::updateControllerRotation(CL_Vec3 controllerOrientation) {
  TransformTreeModel *transformController =
      (TransformTreeModel *)this->controller->getControllerModel()
          ->getComponentList()
          .getComponent("transformTree");
  transformController->setLocalRotation(controllerOrientation);
}
void Image360::updateControllerPosition(CL_Vec3 controllerPosition) {
  TransformTreeModel *transformController =
      (TransformTreeModel *)this->controller->getControllerModel()
          ->getComponentList()
          .getComponent("transformTree");
  transformController->setLocalPosition(controllerPosition);
}
}