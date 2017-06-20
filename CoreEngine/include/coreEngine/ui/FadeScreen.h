#ifndef COREENGINE_FADESCREEN_H
#define COREENGINE_FADESCREEN_H

#include <coreEngine/components/transformTree/ITransformTreeFactory.h>
#include <coreEngine/conf/Types.h>
#include <coreEngine/factory/IModelFactory.h>
#include <coreEngine/factory/IUniformFadeColorFactory.h>
#include <coreEngine/renderObjects/Scene.h>
#include <coreEngine/ui/UIElement.h>

namespace cl {
class FadeScreen : public UIElement {
 public:
  FadeScreen(std::string id, IModelFactory *modelFactory,
             IUniformFadeColorFactory *uniformFadeColorFactory, Scene *scene,
             CL_Vec4 color, ITransformTreeFactory *transformTreeFactory,
             CL_Vec3 &localPosition, CL_Vec3 &localRotation, float width,
             float height, ILoggerFactory *loggerFactory);
  void setColor(CL_Vec4 color);

 private:
  MaterialUniformFadeColor *material;
};
}

#endif  // COREENGINE_FADESCREEN_H