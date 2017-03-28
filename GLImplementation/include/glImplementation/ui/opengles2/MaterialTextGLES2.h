#ifndef GLIMPLEMENTATION_MATERIALTEXTGLES2_H
#define GLIMPLEMENTATION_MATERIALTEXTGLES2_H

#include <coreEngine/ui/MaterialText.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <glImplementation/ui/opengles2/ShaderTextGLES2.h>
#include <glImplementation/ui/TextureTextGL.h>

namespace cl
{
class MaterialTextGLES2 : public MaterialText, public IRenderable
{
  public:
    MaterialTextGLES2(const std::string &sceneId, ShaderTextGLES2 *shader, TextureTextGL *texture, ILoggerFactory *loggerFactory);
    IRenderable *getRenderable();
    void setTextTextureId(CL_GLuint textTextureUniformId); //To be set by respective shader
    void setColorId(CL_GLuint colorUniformId);
    bool initialize();
    void draw();
    void deinitialize();

  private:
    CL_GLuint textTextureUniformId;
    CL_GLuint colorUniformId;
    std::unique_ptr<ILogger> logger = nullptr;
};
}

#endif //GLIMPLEMENTATION_MATERIALTEXTGLES2_H