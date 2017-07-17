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
};
}

#endif  // IMAGE360_BUTTONS_H