#ifndef CORENEGINE_IUNIFORMCOLORFACTORY_H
#define CORENEGINE_IUNIFORMCOLORFACTORY_H

#include <coreEngine/renderObjects/MaterialUniformColor.h>
#include <coreEngine/renderObjects/ShaderUniformColor.h>
#include <memory>

namespace cl {
class IUniformColorFactory {
 public:
  virtual ~IUniformColorFactory() {}
  virtual ShaderUniformColor *getShader(Scene *scene) = 0;
  virtual MaterialUniformColor *getMaterial(ShaderUniformColor *shader,
                                            CL_Vec4 &color) = 0;
};
}

#endif  // CORENEGINE_IUNIFORMCOLORFACTORY_H