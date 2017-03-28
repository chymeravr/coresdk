#ifndef GLIMPLEMENTATION_UNIFORMCOLORFACTORYGLES2_H
#define GLIMPLEMENTATION_UNIFORMCOLORFACTORYGLES2_H

#include <coreEngine/factory/IUniformColorFactory.h>
#include <glImplementation/renderObjects/opengles2/ShaderUniformColorGLES2.h>
#include <glImplementation/renderObjects/opengles2/MaterialUniformColorGLES2.h>

namespace cl
{
class UniformColorFactoryGLES2 : public IUniformColorFactory
{
  public:
    UniformColorFactoryGLES2(ILoggerFactory *loggerFactory);
    ShaderUniformColor *getShader(Scene *scene);
    MaterialUniformColor *getMaterial(ShaderUniformColor *shader, CL_Vec4 &color);

  private:
    ILoggerFactory *loggerFactory = nullptr;
};
}

#endif //GLIMPLEMENTATION_UNIFORMCOLORFACTORYGLES2_H