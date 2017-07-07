#ifndef COREENGINE_RECTANGLEBUILDER_H
#define COREENGINE_RECTANGLEBUILDER_H

#include <coreEngine/modifier/ModelModifier.h>
#include <coreEngine/renderObjects/Model.h>
#include <coreEngine/util/ILogger.h>
#include <coreEngine/util/ILoggerFactory.h>

namespace cl {
class RectangleBuilder {
 public:
  RectangleBuilder(ILoggerFactory *loggerFactory);
  /**
  * Builds a simple rectangle for use with various UI elements
  */
  void buildRectangle(Model *model, float width, float height);

 private:
  std::unique_ptr<ILogger> logger;
};
}

#endif  // COREENGINE_RECTANGLEBUILDER_H