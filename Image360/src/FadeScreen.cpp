#include <image360/FadeScreen.h>
#include <algorithm>

namespace cl {
FadeScreen::FadeScreen(
    ILoggerFactory &loggerFactory, UIFactory &uiFactory,
    TransformTree &parentTransform,
    EventCloseApplicationListener &eventCloseApplicationListener) {
  this->logger = loggerFactory.createLogger("FadeScreen: ");
  this->uiFactory = &uiFactory;
  this->parentTransform = &parentTransform;
  this->eventCloseApplicationListener = &eventCloseApplicationListener;
}

void FadeScreen::initialize(Scene &scene) {
  this->screen = uiFactory->createFadePlanarBackground(
      "fadeScreen", &scene, FADE_SCREEN_COLOR, FADE_SCREEN_POSITION,
      FADE_SCREEN_ROTATION, FADE_SCREEN_WIDTH, FADE_SCREEN_HEIGHT);

  // we need the screen attached to the camera - but it can be attached to
  // anything really
  this->parentTransform->addChild(this->screen->getTransformTree());
}

void FadeScreen::preDraw() {
  if (this->fadeStarted) {
    this->screen->setColor(
        CL_Vec4(0.0, 0.0, 0.0, std::min(this->FADE_ALPHA, 1.0f)));
    if (this->FADE_ALPHA < 1.0) {
      this->FADE_ALPHA += this->FADE_SPEED;
    } else {
      // this->fadeComplete = true;
      this->eventCloseApplicationListener->closeApplication();
    }
  } else if (this->FADE_ALPHA > 0.0f && this->FADE_ALPHA <= 1.0f) {
    this->screen->setColor(
        CL_Vec4(0.0, 0.0, 0.0, std::max(this->FADE_ALPHA, 0.0f)));
    this->FADE_ALPHA -= this->FADE_SPEED / 1.5;
  }
}
}