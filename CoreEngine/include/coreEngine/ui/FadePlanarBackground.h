#ifndef COREENGINE_FADEPLANARBACKGROUND_H
#define COREENGINE_FADEPLANARBACKGROUND_H

#include <coreEngine/components/transformTree/ITransformTreeFactory.h>
#include <coreEngine/conf/Types.h>
#include <coreEngine/factory/IModelFactory.h>
#include <coreEngine/factory/IUniformFadeColorFactory.h>
#include <coreEngine/renderObjects/Scene.h>
#include <coreEngine/ui/UIElement.h>

/*
  We use a separate fade planar background because our scene rendering does
  not take into account any sequencing information while drawing. It renders
  by iterating over shaders as opposed to a client defined rendering order.
  This is done to improve efficiency.
*/

namespace cl {
class FadePlanarBackground : public UIElement {
 public:
  FadePlanarBackground(std::string id, IModelFactory *modelFactory,
                       IUniformFadeColorFactory *uniformFadeColorFactory,
                       Scene *scene, CL_Vec4 color,
                       ITransformTreeFactory *transformTreeFactory,
                       CL_Vec3 &localPosition, CL_Vec3 &localRotation,
                       float width, float height,
                       ILoggerFactory *loggerFactory);
  void setColor(CL_Vec4 color);

 private:
  MaterialUniformFadeColor *material;
};
}

#endif  // COREENGINE_FADEPLANARBACKGROUND_H