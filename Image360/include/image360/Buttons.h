#ifndef IMAGE360_BUTTONS_H
#define IMAGE360_BUTTONS_H

#include <coreEngine/components/gazeDetector/EventGazeListener.h>
#include <coreEngine/components/gazeDetector/GazeDetectorFactory.h>
#include <coreEngine/components/transformTree/ITransformTreeFactory.h>
#include <coreEngine/events/EventCloseApplication.h>
#include <coreEngine/events/EventCloseApplicationListener.h>
#include <coreEngine/factory/IEventGazeListenerFactory.h>
#include <coreEngine/renderObjects/Scene.h>
#include <coreEngine/ui/FontStore.h>
#include <coreEngine/ui/PlanarBackground.h>
#include <coreEngine/ui/TextElement.h>
#include <coreEngine/ui/UIFactory.h>
#include <coreEngine/util/ILogger.h>
#include <coreEngine/util/ILoggerFactory.h>

#include <coreEngine/animation/Animation.h>
#include <image360/ApplicationObject.h>

// todo - separate out different buttons for different purposes.
// todo - change case of constants that are not constants anymore

namespace cl {
class Buttons : public ApplicationObject {
 public:
  Buttons(ILoggerFactory &loggerFactory, UIFactory &uiFactory,
          GazeDetectorContainer &gazeDetectorContainer,
          GazeDetectorFactory &gazeDetectorFactory,
          IEventGazeListenerFactory &eventGazeListenerFactory,
          TransformTree *gazeTransformShooter, std::string fontFolderPath,
          IEvent &eventCloseApplication);

  ~Buttons();

  void initialize(Scene &scene);

  void preDraw();

  inline void setCloseButtonText(std::string newCloseButtonText) {
    this->closeButtonText = newCloseButtonText;
  }

  inline void setActionButtonText(std::string newActionButtonText) {
    this->actionButtonText = newActionButtonText;
  }

  inline std::string getActionButtonText() { return this->actionButtonText; }

  inline EventGazeListener *getActionButtonListener() {
    return this->actionButtonListener.get();
  }

  inline EventGazeListener *getCloseButtonListener() {
    return this->closeButtonListener.get();
  }

  void onClickHandler();

 private:
  // Buttons
  std::unique_ptr<PlanarBackground> actionButtonBackground;
  std::unique_ptr<PlanarBackground> closeButtonBackground;
  std::unique_ptr<EventGazeListener> actionButtonListener;
  std::unique_ptr<EventGazeListener> closeButtonListener;
  std::string closeButtonText = "Close";
  std::string actionButtonText = "Notify Me";

  // std::unique_ptr<GazeDetectorContainer> gazeDetectorContainer;
  // TODO - use the gazedetector to get currently looked at button
  // when handling events
  GazeDetectorContainer *gazeDetectorContainer;
  TransformTree *gazeTransformShooter;

  // Action Button Animations
  std::unique_ptr<Animation<CL_Vec4>> actionButtonOnEnterColorAnimation;
  std::unique_ptr<Animation<CL_Vec3>> actionButtonOnEnterScaleAnimation;

  std::unique_ptr<Animation<CL_Vec4>> actionButtonOnLeaveColorAnimation;
  std::unique_ptr<Animation<CL_Vec3>> actionButtonOnLeaveScaleAnimation;

	// Close Button Animations
  std::unique_ptr<Animation<CL_Vec4>> closeButtonOnEnterColorAnimation;
  std::unique_ptr<Animation<CL_Vec3>> closeButtonOnEnterScaleAnimation;

  std::unique_ptr<Animation<CL_Vec4>> closeButtonOnLeaveColorAnimation;
  std::unique_ptr<Animation<CL_Vec3>> closeButtonOnLeaveScaleAnimation;

  IEvent *eventCloseApplication;

  // Factories
  UIFactory *uiFactory;
  GazeDetectorFactory *gazeDetectorFactory;
  IEventGazeListenerFactory *eventGazeListenerFactory;
  // FontStore *fontStore;
  std::unique_ptr<ILogger> logger;

  // font store component
  std::unique_ptr<FontStore> fontStore;
  // path where the font file is located (.ttf files)
  std::string fontFolderPath = "";

  // CONSTANTS

  float BUTTON_ANIMATION_START_TIME = 0.0f;
  float BUTTON_ANIMATION_END_TIME = 0.25f;

  int FONT_SIZE = 20;
  float FONT_SCALE = 0.025f;
  CL_Vec4 FONT_COLOR = CL_Vec4(1.0f, 1.0f, 1.0f, 1.0f);

  CL_Vec3 ACTION_BUTTON_POSITION = CL_Vec3(-5.1, 0.0, -15.5);
  CL_Vec3 ACTION_BUTTON_ROTATION = CL_Vec3(0.0, 0.0, 0.0);
  
  // animation key frames for scale
  CL_Vec3 ACTION_BUTTON_START_SCALE = CL_Vec3(1.0f, 1.0f, 1.0f);
  CL_Vec3 ACTION_BUTTON_END_SCALE = CL_Vec3(1.25f, 1.25f, 1.25f);

  // animation key grames for color
  CL_Vec4 ACTION_BUTTON_START_COLOR = CL_Vec4(0.0f, 0.0f, 0.0f, 0.7f);
  CL_Vec4 ACTION_BUTTON_END_COLOR = CL_Vec4(0.0f, 0.56f, 0.80f, 0.70f);

  // animation key frames for scale
  CL_Vec3 CLOSE_BUTTON_START_SCALE = CL_Vec3(1.0f, 1.0f, 1.0f);
  CL_Vec3 CLOSE_BUTTON_END_SCALE = CL_Vec3(1.24f, 1.24f, 1.24f);

  // animation key grames for color
  CL_Vec4 CLOSE_BUTTON_START_COLOR = CL_Vec4(0.0f, 0.0f, 0.0f, 0.699f);
  CL_Vec4 CLOSE_BUTTON_END_COLOR = CL_Vec4(0.0f, 0.56f, 0.80f, 0.69f);

  float ACTION_BUTTON_WIDTH = 3.0f;
  float ACTION_BUTTON_HEIGHT = 1.0f;

  // initial button color set to start button colors
  CL_Vec4 actionButtonColor = ACTION_BUTTON_START_COLOR;
  CL_Vec3 actionButtonScale = ACTION_BUTTON_START_SCALE;

  // initial button color set to close button colors
  CL_Vec4 closeButtonColor = CLOSE_BUTTON_START_COLOR;
  CL_Vec3 closeButtonScale = CLOSE_BUTTON_START_SCALE;

  CL_Vec3 ACTION_TEXT_POSITION = CL_Vec3(-1.0f, -0.2f, 0.001);
  CL_Vec3 ACTION_TEXT_ROTATION = CL_Vec3(0.0, 0.0, 0.0);

  CL_Vec3 CLOSE_BUTTON_POSITION = CL_Vec3(5.1, 0.0, -15.5);
  CL_Vec3 CLOSE_BUTTON_ROTATION = CL_Vec3(0.0, 0.0, 0.0);
  CL_Vec3 CLOSE_BUTTON_SCALE = CL_Vec3(1.0f, 1.0f, 1.0f);
  CL_Vec4 CLOSE_BUTTON_COLOR = CL_Vec4(0.0, 0.0, 0.0, 0.69);
  float CLOSE_BUTTON_WIDTH = 3.0f;
  float CLOSE_BUTTON_HEIGHT = 1.0f;

  CL_Vec3 CLOSE_TEXT_POSITION = CL_Vec3(-0.5f, -0.2f, 0.001);
  CL_Vec3 CLOSE_TEXT_ROTATION = CL_Vec3(0.0, 0.0, 0.0);

  CL_Vec3 ACTION_BUTTON_GAZE_DETECTOR_ORIGIN = CL_Vec3(0.0f, 0.0f, 0.0f);
  CL_Vec3 ACTION_BUTTON_GAZE_DETECTOR_LOOK_AT = CL_Vec3(0.0f, 0.0f, -1.0f);
  float ACTION_BUTTON_GAZE_DETECTOR_LENGTH_X = 3.0f;
  float ACTION_BUTTON_GAZE_DETECTOR_LENGTH_Y = 1.0f;
  float ACTION_BUTTON_GAZE_DETECTOR_LENGTH_Z = 0.00001f;

  CL_Vec3 CLOSE_BUTTON_GAZE_DETECTOR_ORIGIN = CL_Vec3(0.0f, 0.0f, 0.0f);
  CL_Vec3 CLOSE_BUTTON_GAZE_DETECTOR_LOOK_AT = CL_Vec3(0.0f, 0.0f, -1.0f);
  float CLOSE_BUTTON_GAZE_DETECTOR_LENGTH_X = 3.0f;
  float CLOSE_BUTTON_GAZE_DETECTOR_LENGTH_Y = 1.0f;
  float CLOSE_BUTTON_GAZE_DETECTOR_LENGTH_Z = 0.00001f;
};
}

#endif  // IMAGE360_BUTTONS_H