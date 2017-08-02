#ifndef IMAGE360_FADESCREEN_H
#define IMAGE360_FADESCREEN_H

#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <coreEngine/components/gazeDetector/GazeDetectorFactory.h>
#include <coreEngine/components/transformTree/ITransformTreeFactory.h>
#include <coreEngine/factory/IEventGazeListenerFactory.h>
#include <coreEngine/renderObjects/Scene.h>
#include <coreEngine/ui/FontStore.h>
#include <coreEngine/ui/PlanarBackground.h>
#include <coreEngine/ui/TextElement.h>
#include <coreEngine/ui/UIFactory.h>
#include <coreEngine/util/ILogger.h>
#include <coreEngine/util/ILoggerFactory.h>

#include <coreEngine/events/EventCloseApplicationListener.h>
#include <image360/ApplicationObject.h>

// Animation Headers
#include <coreEngine/animation/Animation.h>
#include <coreEngine/animation/ChronoTimeKeeper.h>
#include <coreEngine/animation/LinearInterpolator.h>

namespace cl {
class FadeScreen : public ApplicationObject {
 public:
  FadeScreen(ILoggerFactory &loggerFactory, UIFactory &uiFactory,
             TransformTree &parentTransform,
             EventCloseApplicationListener &eventCloseApplicationListener);
  ~FadeScreen();
  void initialize(Scene &scene);
  void preDraw();

  void beginFade() {
    this->fadeStarted = true;
    animation->start();
  }

  // bool isFadeComplete() { return this->fadeComplete; }

 private:
  // Factories
  UIFactory *uiFactory;
  std::unique_ptr<FadePlanarBackground> screen;
  std::unique_ptr<ILogger> logger;

  TransformTree *parentTransform;

  EventCloseApplicationListener *eventCloseApplicationListener;

  bool fadeStarted = false;
  // bool fadeComplete = false;

  // CONSTANTS
  // FadeScreen Params
  CL_Vec3 FADE_SCREEN_POSITION = CL_Vec3(0.0f, 0.0f, -0.2f);
  CL_Vec3 FADE_SCREEN_ROTATION = CL_Vec3(0.0f, 0.0f, 0.0f);
  CL_Vec3 FADE_SCREEN_COLOR = CL_Vec3(0.0f, 0.0f, 0.0f);

  float FADE_SCREEN_WIDTH = 20.0f;
  float FADE_SCREEN_HEIGHT = 20.0f;

  // todo ~ in future, the animator will take care of this
  CL_GLfloat FADE_ALPHA = 0.0f;
  CL_GLfloat FADE_SPEED = 0.01f;

  std::unique_ptr<Animation<float>> animation;
};
}

#endif  // IMAGE360_FADESCREEN_H