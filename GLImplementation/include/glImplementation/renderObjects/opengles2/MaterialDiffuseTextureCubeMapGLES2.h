#ifndef GLIMPLEMENTATION_MATERIALDIFFUSETEXTURECUBEMAPGLES2_H
#define GLIMPLEMENTATION_MATERIALDIFFUSETEXTURECUBEMAPGLES2_H

#include <coreEngine/renderObjects/MaterialDiffuseTextureCubeMap.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <conf/GLTypes.h>
#include <glImplementation/renderObjects/opengles2/ShaderDiffuseTextureCubeMapGLES2.h>

namespace cl
{
class MaterialDiffuseTextureCubeMapGLES2 : public MaterialDiffuseTextureCubeMap, public IRenderable
{
  public:
    MaterialDiffuseTextureCubeMapGLES2(const std::string &sceneId, ShaderDiffuseTextureCubeMapGLES2 *shader, ILoggerFactory *loggerFactory);
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

#endif //GLIMPLEMENTATION_MATERIALDIFFUSETEXTURECUBEMAPGLES2_H