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

// todo :
// 1. clean up event list
// 2. implement component interface

namespace cl {
Image360::Image360(std::unique_ptr<IRenderer> renderer,
                   ISceneFactory &sceneFactory,
                   ITransformTreeFactory &transformTreeFactory,
                   ICameraFactory &cameraFactory, IEventQueue &eventQueue,
                   ILoggerFactory &loggerFactory, UIFactory &uiFactory,
                   //  GazeDetectorFactory &gazeDetectorFactory,
                   GazeDetectorContainer &gazeDetectorContainer,
                   IEventGazeListenerFactory &gazeEventListenerFactory) {
  assert(renderer != nullptr);

  this->renderer = std::move(renderer);
  this->eventQueue = &eventQueue;

  this->sceneFactory = &sceneFactory;
  this->transformTreeFactory = &transformTreeFactory;
  this->cameraFactory = &cameraFactory;
  this->loggerFactory = &loggerFactory;
  this->uiFactory = &uiFactory;
  this->eventGazeListenerFactory = &gazeEventListenerFactory;

  this->logger = this->loggerFactory->createLogger("Image360::");

  this->gazeDetectorContainer = &gazeDetectorContainer;
}

void Image360::start() {
  this->renderer->start();
  this->logger->log(LOG_INFO, "Application started.");
}

void Image360::initialize() {
  // decide if camera should be initialized separately and added as a component
  // to this

  // scene stuff
  this->scene = sceneFactory->create("Image360Scene");
  assert(scene != nullptr);

  this->scene->setBackgroundColor(SCENE_BACKGROUND_COLOR);

  // TODO : fix bug that causes the sequence of addition of components to matter
  //        in the rendering.
  for (int i = 0; i < image360Components.size(); i++) {
    image360Components[i]->initialize(*this->scene);
  }
}

// void Image360::initCameraReticle() {
// create a reticle attached to the main camera
/*TransformTree *transformTreeCamera =
    this->fpsCamera->getCameraTransformTree();
this->reticle = this->uiFactory->createReticle(
    "reticle", scene.get(), transformTreeCamera, CAMERA_RETICLE_COLOR);*/
// }

// void Image360::initController() {
// this->controller->initialize(*scene);
// this->gazeTransformShooter = this->controller->getGazeTransformSource();
// }

// void Image360::initFadeScreen() {
// fade screen initialization
/*this->fadeScreen = uiFactory->createFadeScreen(
    "fadeScreen", scene.get(), FADE_SCREEN_COLOR, FADE_SCREEN_POSITION,
    FADE_SCREEN_ROTATION, FADE_SCREEN_WIDTH, FADE_SCREEN_HEIGHT);
TransformTree *transformTreeCamera =
    this->fpsCamera->getCameraTransformTree();
transformTreeCamera->addChild(this->fadeScreen->getTransformTree());*/
// }

void Image360::initComplete() { this->renderer->initialize(scene.get()); }

void Image360::update() { renderer->update(); }

void Image360::drawInit() {
  // if (this->fadeStarted) {
  //   if (this->fadeScreen != nullptr) {
  //     this->fadeScreen->setColor(CL_Vec4(0.0, 0.0, 0.0, this->FADE_ALPHA));
  //     if (this->FADE_ALPHA < 1.0) {
  //       this->FADE_ALPHA += this->FADE_SPEED;
  //     } else {
  //       this->fadeComplete = true;
  //     }
  //   } else {
  //     this->fadeComplete = true;
  //   }
  // } else if (this->FADE_ALPHA > 0.0f && this->FADE_ALPHA <= 1.0f) {
  //   if (this->fadeScreen != nullptr) {
  //     this->fadeScreen->setColor(CL_Vec4(0.0, 0.0, 0.0, this->FADE_ALPHA));
  //   }
  //   this->FADE_ALPHA -= this->FADE_SPEED / 1.5;
  // }

  for (int i = 0; i < image360Components.size(); i++) {
    image360Components[i]->preDraw();
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
  // this->stereoSphere->preDrawLeft();
  renderer->drawLeft(scene.get());
}

void Image360::drawStereoRight() {
  // this->stereoSphere->preDrawRight();
  renderer->drawRight(scene.get());
}

void Image360::drawComplete() { renderer->drawComplete(); }

void Image360::deinitialize() { renderer->deinitialize(scene.get()); }
void Image360::stop() { renderer->stop(); }

void Image360::pause() { renderer->pause(); }

void Image360::resume() { renderer->resume(); }

Reticle *Image360::getParentReticle() {
  /*if (this->controller->getControllerReticle() != nullptr) {
    return this->controller->getControllerReticle();
  }*/
  return this->reticleBase.get();
}

IRenderer *Image360::getRenderer() { return this->renderer.get(); }
}