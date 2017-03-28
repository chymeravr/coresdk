#ifndef GLIMPLEMENTATION_MATERIALUNIFORMCOLORGLES2_H
#define GLIMPLEMENTATION_MATERIALUNIFORMCOLORGLES2_H

#include <coreEngine/renderObjects/MaterialUniformColor.h>
#include <glImplementation/renderObjects/opengles2/ShaderUniformColorGLES2.h>

namespace cl
{
class MaterialUniformColorGLES2 : public MaterialUniformColor, public IRenderable
{
  public:
    MaterialUniformColorGLES2(const std::string &sceneId, ShaderUniformColorGLES2 *shader, ILoggerFactory *loggerFactory, CL_Vec4 &color);
    IRenderable *getRenderable();
    void setColorUniformId(CL_GLuint colorUniformId); //To be set by respective shader
    bool initialize();
    void draw();
    void deinitialize();

  private:
    std::unique_ptr<ILogger> logger;
    CL_GLuint colorUniformId;
};
}

#endif //GLIMPLEMENTATION_MATERIALUNIFORMCOLORGLES2_H