#ifndef CORENEGINE_IUNIFORMFADECOLORFACTORY_H
#define CORENEGINE_IUNIFORMFADECOLORFACTORY_H

#include <coreEngine/renderObjects/MaterialUniformFadeColor.h>
#include <coreEngine/renderObjects/ShaderUniformFadeColor.h>
#include <memory>

namespace cl {
class IUniformFadeColorFactory {
 public:
  virtual ~IUniformFadeColorFactory() {}
  virtual ShaderUniformFadeColor *getShader(Scene *scene) = 0;
  virtual MaterialUniformFadeColor *getMaterial(ShaderUniformFadeColor *shader,
                                                CL_Vec4 &color) = 0;
};
}

#endif  // CORENEGINE_IUNIFORMFADECOLORFACTORY_H