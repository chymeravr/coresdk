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

  auto interpolator = std::unique_ptr<LinearInterpolator<float>>(
      new LinearInterpolator<float>());
  auto timeKeeper = std::unique_ptr<ChronoTimeKeeper>(new ChronoTimeKeeper());
  animation = std::unique_ptr<Animation<float>>(
      new Animation<float>(loggerFactory, this->FADE_ALPHA,
                           std::move(interpolator), std::move(timeKeeper)));
}

FadeScreen::~FadeScreen() {
  this->logger->log(LOG_DEBUG, "FadeScreen Destructor");
}

void FadeScreen::initialize(Scene &scene) {
  CL_Vec4 fadeScreenColor = CL_Vec4(this->FADE_SCREEN_COLOR, this->FADE_ALPHA);
  this->screen = uiFactory->createFadePlanarBackground(
      "fadeScreen", &scene, fadeScreenColor, FADE_SCREEN_POSITION,
      FADE_SCREEN_ROTATION, FADE_SCREEN_WIDTH, FADE_SCREEN_HEIGHT);

  // we need the screen attached to the camera - but it can be attached to
  // anything really
  this->parentTransform->addChild(this->screen->getTransformTree());
  std::vector<float> alphaFrames = {0.0f, 1.0f};
  std::vector<float> timeFrames = {0.0f, 0.75f};
  this->animation->setKeyFrames(alphaFrames, timeFrames);
}

void FadeScreen::preDraw() {
  // if (this->fadeStarted) {
  //   this->screen->setColor(
  //       CL_Vec4(0.0, 0.0, 0.0, std::min(this->FADE_ALPHA, 1.0f)));
  //   if (this->FADE_ALPHA < 1.0) {
  //     this->FADE_ALPHA += this->FADE_SPEED;
  //   } else {
  //     // this->fadeComplete = true;
  //     this->eventCloseApplicationListener->closeApplication();
  //   }
  // } else if (this->FADE_ALPHA > 0.0f && this->FADE_ALPHA <= 1.0f) {
  //   this->screen->setColor(
  //       CL_Vec4(0.0, 0.0, 0.0, std::max(this->FADE_ALPHA, 0.0f)));
  //   this->FADE_ALPHA -= this->FADE_SPEED / 1.5;
  // }

  if (this->fadeStarted) {
    this->animation->update();
    auto newScreenColor =
        CL_Vec4(0.0, 0.0, 0.0, std::min(this->FADE_ALPHA, 1.0f));
    this->screen->setColor(newScreenColor);
    if (!this->animation->isAnimationRunning()) {
      logger->log(LOG_DEBUG, "Animation Complete");
      this->eventCloseApplicationListener->closeApplication();
    }
  }
  // else if (this->FADE_ALPHA > 0.0f && this->FADE_ALPHA <= 1.0f) {
  //   this->screen->setColor(
  //       CL_Vec4(0.0, 0.0, 0.0, std::max(this->FADE_ALPHA, 0.0f)));
  //   this->FADE_ALPHA -= this->FADE_SPEED / 1.5;
  // }
}
}