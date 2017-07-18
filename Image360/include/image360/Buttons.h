#ifndef IMAGE360_BUTTONS_H
#define IMAGE360_BUTTONS_H

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

namespace cl {
class Buttons {
 public:
  Buttons(ILoggerFactory &loggerFactory, UIFactory &uiFactory,
          GazeDetectorFactory &gazeDetectorFactory, FontStore &fontStore,
          IEventGazeListenerFactory &eventGazeListenerFactory);
  void initialize(Scene &scene, TransformTree &gazeTransformTarget);

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

 private:
  // Buttons
  std::unique_ptr<PlanarBackground> actionButtonBackground;
  std::unique_ptr<PlanarBackground> closeButtonBackground;
  std::unique_ptr<EventGazeListener> actionButtonListener;
  std::unique_ptr<EventGazeListener> closeButtonListener;
  std::string closeButtonText = "Close";
  std::string actionButtonText = "Notify Me";

  std::unique_ptr<GazeDetectorContainer> gazeDetectorContainer;
  // TransformTree *gazeTransformTarget;

  // Factories
  UIFactory *uiFactory;
  GazeDetectorFactory *gazeDetectorFactory;
  IEventGazeListenerFactory *eventGazeListenerFactory;
  FontStore *fontStore;
  std::unique_ptr<ILogger> logger;

  // CONSTANTS
  int FONT_SIZE = 20;
  float FONT_SCALE = 0.025f;
  CL_Vec4 FONT_COLOR = CL_Vec4(1.0f, 1.0f, 1.0f, 1.0f);

  CL_Vec3 ACTION_BUTTON_POSITION = CL_Vec3(-5.1, 0.0, -15.5);
  CL_Vec3 ACTION_BUTTON_ROTATION = CL_Vec3(0.0, 0.0, 0.0);
  CL_Vec4 ACTION_BUTTON_COLOR = CL_Vec4(0.0, 0.0, 0.0, 0.7);
  float ACTION_BUTTON_WIDTH = 3.0f;
  float ACTION_BUTTON_HEIGHT = 1.0f;

  CL_Vec3 ACTION_TEXT_POSITION = CL_Vec3(-1.0f, -0.2f, 0.001);
  CL_Vec3 ACTION_TEXT_ROTATION = CL_Vec3(0.0, 0.0, 0.0);

  CL_Vec3 CLOSE_BUTTON_POSITION = CL_Vec3(5.1, 0.0, -15.5);
  CL_Vec3 CLOSE_BUTTON_ROTATION = CL_Vec3(0.0, 0.0, 0.0);
  CL_Vec4 CLOSE_BUTTON_COLOR = CL_Vec4(0.0, 0.0, 0.0, 0.7);
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