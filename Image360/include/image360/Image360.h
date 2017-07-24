#ifndef IMAGE360_IMAGE360_H
#define IMAGE360_IMAGE360_H

#include <coreEngine/IRenderer.h>
#include <coreEngine/events/EventKeyPressListener.h>
#include <coreEngine/events/EventPassiveMouseMotionListener.h>
#include <coreEngine/events/IEventQueue.h>
#include <coreEngine/factory/ICameraFactory.h>
#include <coreEngine/factory/ISceneFactory.h>

#include <coreEngine/components/gazeDetector/GazeDetectorFactory.h>
#include <coreEngine/components/transformTree/ITransformTreeFactory.h>
#include <coreEngine/modifier/Image.h>
#include <coreEngine/ui/UIFactory.h>

#include <coreEngine/factory/IEventGazeListenerFactory.h>

#include <image360/ApplicationObject.h>
// #include <image360/Buttons.h>
// #include <image360/Constants.h>
// #include <image360/Controller.h>
// #include <image360/FPSCamera.h>
// #include <image360/MonoCubeMap.h>
// #include <image360/MonoSphere.h>
// #include <image360/StereoSphere.h>

// todo
// 1. remove fontFolderPath from here
// 2. remove all factories not directly used by image360
// 3. Image360 is no longer owner of factories ~ individual components are.
// 4.

namespace cl {

class Image360 {
 public:
  Image360(std::unique_ptr<IRenderer> renderer, ISceneFactory &sceneFactory,
           ITransformTreeFactory &transformTreeFactory,
           ICameraFactory &cameraFactory, IEventQueue &eventQueue,
           ILoggerFactory &loggerFactory, UIFactory &uiFactory,
           GazeDetectorContainer &gazeDetectorContainer,
           IEventGazeListenerFactory &gazeEventListenerFactory);

  void start();

  void initialize();

  // call initComplete after you have initialized the scene graph
  void initComplete();

  // TODO : review update functions
  void update();

  // draw is split into 4 steps - init, left eye, right eye, complete
  // draw init common stuff
  void drawInit();
  // draw mono - no distinction between rendering for left and right eye
  void drawMono();
  // draw stereo - eye specific stuff - camera, models etc.
  void drawStereoRight();
  void drawStereoLeft();
  void drawComplete();

  void deinitialize();
  void stop();
  void pause();
  void resume();

  IRenderer *getRenderer();

  void setIsControllerPresent(bool isControllerPresent) {
    this->isControllerPresent = isControllerPresent;
  }

  // void beginFade() { fadeStarted = true; }

  // bool isFadeComplete() { return this->fadeComplete; }

  Reticle *getParentReticle();

  TransformTree *getGazeTransformShooter() {
    return this->gazeTransformShooter;
  }

  void addApplicationObject(ApplicationObject *appObject) {
    this->image360Components.push_back(appObject);
  }

 private:
  // Image360 Class will initialze a set of renderables into a scene
  std::unique_ptr<Scene> scene;

  // All components of Image360 Application are stored in a container
  // iterate and initialize them
  std::vector<ApplicationObject *> image360Components;

  // TODO : try and remove this bool flag ~ considered bad practice
  bool isControllerPresent = false;

  // logging utils
  ILoggerFactory *loggerFactory;
  std::unique_ptr<ILogger> logger;

  // Renderer Encapsulate the draw calls to the GPU rendering library
  // (opengl, // directx, vulkan)
  // This is what allows our application to be cross platform
  std::unique_ptr<IRenderer> renderer;

  // event queue contains events to be processed at each draw call
  IEventQueue *eventQueue;

  // These helper factories abstract instantiation of component
  ISceneFactory *sceneFactory;
  ITransformTreeFactory *transformTreeFactory;
  ICameraFactory *cameraFactory;
  UIFactory *uiFactory;
  GazeDetectorFactory *gazeDetectorFactory;
  IEventGazeListenerFactory *eventGazeListenerFactory;

  // fade screen component
  /*std::unique_ptr<FadeScreen> fadeScreen;
  bool fadeStarted = false;
  bool fadeComplete = false;*/

  // Reticle component
  std::unique_ptr<Reticle> reticle;
  std::unique_ptr<Reticle> reticleBase;

  // contains all our gaze detection boxes

  TransformTree *gazeTransformShooter;

  // TODO: maybe this should be called the gazeTransformSource??
  GazeDetectorContainer *gazeDetectorContainer;

  // CONSTANTS
  CL_Vec4 SCENE_BACKGROUND_COLOR = CL_Vec4(0.0f, 0.0f, 0.4f, 0.0f);
  CL_Vec4 CAMERA_RETICLE_COLOR = CL_Vec4(0.0f, 1.0f, 0.0f, 1.0f);
};
}

#endif  // IMAGE360_IMAGE360_H