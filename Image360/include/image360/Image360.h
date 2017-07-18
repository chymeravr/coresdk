#ifndef IMAGE360_IMAGE360_H
#define IMAGE360_IMAGE360_H

#include <coreEngine/IRenderer.h>
#include <coreEngine/events/EventKeyPressListener.h>
#include <coreEngine/events/EventPassiveMouseMotionListener.h>
#include <coreEngine/events/IEventQueue.h>
#include <coreEngine/factory/ICameraFactory.h>
#include <coreEngine/factory/IDiffuseTextureCubeMapFactory.h>
#include <coreEngine/factory/IDiffuseTextureFactory.h>
#include <coreEngine/factory/IModelFactory.h>
#include <coreEngine/factory/ISceneFactory.h>

#include <coreEngine/components/gazeDetector/GazeDetectorFactory.h>
#include <coreEngine/components/transformTree/ITransformTreeFactory.h>
#include <coreEngine/modifier/Image.h>
#include <coreEngine/ui/UIFactory.h>

#include <coreEngine/factory/IEventGazeListenerFactory.h>

#include <image360/Buttons.h>
#include <image360/Constants.h>
#include <image360/Controller.h>
#include <image360/MonoCubeMap.h>
#include <image360/MonoSphere.h>
#include <image360/StereoSphere.h>

namespace cl {

class Image360 {
 public:
  Image360(std::unique_ptr<IRenderer> renderer,
           std::unique_ptr<ISceneFactory> sceneFactory,
           std::unique_ptr<IModelFactory> modelFactory,
           std::unique_ptr<IDiffuseTextureFactory> diffuseTextureFactory,
           std::unique_ptr<IDiffuseTextureCubeMapFactory>
               diffuseTextureCubeMapFactory,
           std::unique_ptr<ITransformTreeFactory> transformTreeFactory,
           std::unique_ptr<ICameraFactory> cameraFactory,
           IEventQueue *eventQueue, ILoggerFactory *loggerFactory,
           std::unique_ptr<UIFactory> uiFactory,
           std::unique_ptr<GazeDetectorFactory> gazeDetectorFactory,
           std::unique_ptr<IEventGazeListenerFactory> gazeEventListenerFactory,
           std::string fontFolderPath);

  void start();

  void initialize();

  // use mono init funtions if you have a mono image360

  void initMonoEquirectangularView(std::unique_ptr<Image> textureImage);
  void initMonoCubeMapSingleTextureView(std::unique_ptr<Image> textureImage);
  void initMonoCubeMapSixTextureView(
      std::vector<std::unique_ptr<Image>> textureImage);

  // use stereo init functions if you have a stereo image360 (Top Bottom => Left
  // Right)

  void initStereoEquirectangularView(std::unique_ptr<Image> textureImage);
  // note - stereo cubemaps not implemented because we did not find them
  // popularly
  // used despite John Cormack favouring the format

  // Addition User Interface Objects ~ buttons, reticle, controller,
  // controller laser, fadein/out screen
  void initUIButtons();
  void initCameraReticle();
  void initControllerReticle();
  void initController(std::unique_ptr<Image> controllerImage,
                      std::string controllerModelPath);
  void initControllerLaser(std::unique_ptr<Image> laserBeamImage);
  void initFadeScreen();
  // call initComplete after you have initialized the scene graph
  void initComplete();

  // TODO : review update functions
  void update();
  void updateControllerQuaternion(CL_Quat controllerOrientation);
  void updateControllerRotation(CL_Vec3 controllerRotation);
  void updateControllerPosition(CL_Vec3 controllerPosition);
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

  void beginFade() { fadeStarted = true; }

  bool isFadeComplete() { return this->fadeComplete; }

  Reticle *getParentReticle();

  Buttons *getButtons() { return this->buttons.get(); }
  Camera *getCamera() { return this->camera; }

  Model *getControllerModel() { return this->controller->getControllerModel(); }

  EventGazeListener *getActionButtonListener() {
    return this->buttons->getActionButtonListener();
  }

  EventGazeListener *getCloseButtonListener() {
    return this->buttons->getCloseButtonListener();
  }

  TransformTree *getGazeTransformTarget() { return this->gazeTransformTarget; }

 private:
  // Image360 Class will initialze a set of renderables into a scene
  std::unique_ptr<Scene> scene;

  // Main camera for our application
  Camera *camera;

  // TODO : try and remove this bool flag ~ considered bad practice
  bool isControllerPresent = false;

  //   // Controller Component
  std::unique_ptr<Controller> controller;
  // mono spherical rendering component
  std::unique_ptr<MonoSphere> monoSphere;

  // mono cubemap rendering component
  std::unique_ptr<MonoCubeMap> monoCubeMap;

  // stereo spherical rendering component
  std::unique_ptr<StereoSphere> stereoSphere;

  // logging utils
  ILoggerFactory *loggerFactory;
  std::unique_ptr<ILogger> logger;

  // Renderer Encapsulate the draw calls to the GPU rendering library (opengl,
  // directx, vulkan)
  // This is what allows our application to be cross platform
  std::unique_ptr<IRenderer> renderer;

  // event queue contains events to be processed at each draw call
  IEventQueue *eventQueue;

  // These helper factories abstract instantiation of component
  std::unique_ptr<ISceneFactory> sceneFactory;
  std::unique_ptr<IModelFactory> modelFactory;
  std::unique_ptr<IDiffuseTextureFactory> diffuseTextureFactory;
  std::unique_ptr<IDiffuseTextureCubeMapFactory> diffuseTextureCubeMapFactory;
  std::unique_ptr<ITransformTreeFactory> transformTreeFactory;
  std::unique_ptr<ICameraFactory> cameraFactory;
  std::unique_ptr<UIFactory> uiFactory;
  std::unique_ptr<GazeDetectorFactory> gazeDetectorFactory;
  std::unique_ptr<IEventGazeListenerFactory> eventGazeListenerFactory;

  // font store component
  std::unique_ptr<FontStore> fontStore;
  // path where the font file is located (.ttf files)
  std::string fontFolderPath = "";

  // Buttons
  std::unique_ptr<Buttons> buttons;

  // fade screen component
  std::unique_ptr<FadeScreen> fadeScreen;
  bool fadeStarted = false;
  bool fadeComplete = false;

  // Reticle component
  std::unique_ptr<Reticle> reticle;
  std::unique_ptr<Reticle> reticleBase;

  // contains all our gaze detection boxes
  // std::unique_ptr<GazeDetectorContainer> gazeDetectorContainer;

  // TODO: maybe this should be called the gazeTransformSource??
  TransformTree *gazeTransformTarget;

  // CONSTANTS
  CL_Vec4 SCENE_BACKGROUND_COLOR = CL_Vec4(0.0f, 0.0f, 0.4f, 0.0f);
  CL_Vec4 CAMERA_RETICLE_COLOR = CL_Vec4(0.0f, 1.0f, 0.0f, 1.0f);

  // Initial values of camera params ~ hmds provide these
  // through their apis and should be reset to use them as such
  float ASPECT_RATIO = 1.5f;
  float NEAR_PLANE = 0.1f;
  float FAR_PLANE = 1000.0f;
  float FOV = 1.75f;
  CL_Vec3 CAMERA_POSITION = CL_Vec3(0.0f, 0.0f, 0.0f);
  CL_Vec3 CAMERA_ROTATION = CL_Vec3(0.0f, 0.0f, 0.0f);

  // FadeScreen Params
  CL_Vec3 FADE_SCREEN_POSITION = CL_Vec3(0.0f, 0.0f, -2.0f);
  CL_Vec3 FADE_SCREEN_ROTATION = CL_Vec3(0.0f, 0.0f, 0.0f);
  CL_Vec4 FADE_SCREEN_COLOR = CL_Vec4(0.0f, 0.0f, 0.0f, 0.0f);
  float FADE_SCREEN_WIDTH = 20.0f;
  float FADE_SCREEN_HEIGHT = 20.0f;
  CL_GLfloat FADE_ALPHA = 1.0f;
  CL_GLfloat FADE_SPEED = 0.01f;
};
}

#endif  // IMAGE360_IMAGE360_H