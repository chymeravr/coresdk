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
Image360::Image360(std::unique_ptr<IRenderer> renderer,
                   ISceneFactory &sceneFactory, IEventQueue &eventQueue,
                   ILoggerFactory &loggerFactory,
                   GazeDetectorContainer &gazeDetectorContainer) {
  assert(renderer != nullptr);

  this->renderer = std::move(renderer);
  this->eventQueue = &eventQueue;

  this->sceneFactory = &sceneFactory;
  this->loggerFactory = &loggerFactory;

  this->logger = this->loggerFactory->createLogger("Image360::");

  this->gazeDetectorContainer = &gazeDetectorContainer;
}

void Image360::start() {
  this->renderer->start();
  this->logger->log(LOG_INFO, "Application started.");
}

void Image360::initialize() {
  // scene instantiation
  this->scene = sceneFactory->create("Image360Scene");
  assert(scene != nullptr);

  this->scene->setBackgroundColor(SCENE_BACKGROUND_COLOR);

  // Note : our current renderers do not render scene objects in any pre
  //        defined order. This introduces some artifacts due to sequence
  //        of initialization. Currently, we have ignored these.
  for (int i = 0; i < image360Components.size(); i++) {
    image360Components[i]->initialize(*this->scene);
  }

  this->renderer->initialize(scene.get());
}

void Image360::update() { renderer->update(); }

void Image360::drawInit() {
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
  for (int i = 0; i < image360StereoComponents.size(); i++) {
    image360StereoComponents[i]->preDrawLeft();
  }
  renderer->drawLeft(scene.get());
}

void Image360::drawStereoRight() {
  for (int i = 0; i < image360StereoComponents.size(); i++) {
    image360StereoComponents[i]->preDrawRight();
  }
  renderer->drawRight(scene.get());
}

void Image360::drawComplete() { renderer->drawComplete(); }

void Image360::deinitialize() { renderer->deinitialize(scene.get()); }
void Image360::stop() { renderer->stop(); }

void Image360::pause() { renderer->pause(); }

void Image360::resume() { renderer->resume(); }

IRenderer *Image360::getRenderer() { return this->renderer.get(); }
}