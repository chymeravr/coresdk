#ifndef GLIMPLEMENTATION_UNIFORMFADECOLORFACTORYGL_H
#define GLIMPLEMENTATION_UNIFORMFADECOLORFACTORYGL_H

#include <coreEngine/factory/IUniformFadeColorFactory.h>
#include <glImplementation/renderObjects/MaterialUniformFadeColorGL.h>
#include <glImplementation/renderObjects/ShaderUniformFadeColorGL.h>

namespace cl {
class UniformFadeColorFactoryGL : public IUniformFadeColorFactory {
 public:
  UniformFadeColorFactoryGL(ILoggerFactory *loggerFactory);
  ShaderUniformFadeColor *getShader(Scene *scene);
  MaterialUniformFadeColor *getMaterial(ShaderUniformFadeColor *shader,
                                        CL_Vec4 &color);

 private:
  ILoggerFactory *loggerFactory = nullptr;
};
}

#endif  // GLIMPLEMENTATION_UNIFORMFADECOLORFACTORYGL_H