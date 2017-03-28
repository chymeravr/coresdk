#ifndef GLIMPLEMENTATION_MATERIALDIFFUSETEXTUREGLES2_H
#define GLIMPLEMENTATION_MATERIALDIFFUSETEXTUREGLES2_H

#include <coreEngine/renderObjects/MaterialDiffuseTexture.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <conf/GLTypes.h>
#include <glImplementation/renderObjects/opengles2/ShaderDiffuseTextureGLES2.h>

namespace cl
{
class MaterialDiffuseTextureGLES2 : public MaterialDiffuseTexture, public IRenderable
{
  public:
    MaterialDiffuseTextureGLES2(const std::string &sceneId, ShaderDiffuseTextureGLES2 *shader, ILoggerFactory *loggerFactory);
    IRenderable *getRenderable();
    void setDiffuseTextureId(CL_GLuint diffuseTextureUniformId); //To be set by respective shader
    bool initialize();
    void draw();
    void deinitialize();

  private:
    std::unique_ptr<ILogger> logger;
    CL_GLuint diffuseTextureUniformId;
};
}

#endif //GLIMPLEMENTATION_MATERIALDIFFUSETEXTUREGLES2_H