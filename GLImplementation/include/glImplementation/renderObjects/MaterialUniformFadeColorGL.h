#ifndef GLIMPLEMENTATION_MATERIALUNIFORMFADECOLORGL_H
#define GLIMPLEMENTATION_MATERIALUNIFORMFADECOLORGL_H

#include <coreEngine/renderObjects/MaterialUniformFadeColor.h>
#include <glImplementation/renderObjects/ShaderUniformFadeColorGL.h>

namespace cl
{
class MaterialUniformFadeColorGL : public MaterialUniformFadeColor, public IRenderable
{
  public:
    MaterialUniformFadeColorGL(const std::string &sceneId, ShaderUniformFadeColorGL *shader,
                               ILoggerFactory *loggerFactory, CL_Vec4 &color);
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

#endif //GLIMPLEMENTATION_MATERIALUNIFORMFADECOLORGL_H