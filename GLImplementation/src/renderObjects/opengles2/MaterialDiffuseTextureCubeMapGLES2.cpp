#include <glImplementation/renderObjects/opengles2/MaterialDiffuseTextureCubeMapGLES2.h>
#include <glImplementation/renderObjects/TextureCubeMapGL.h>

namespace cl
{
MaterialDiffuseTextureCubeMapGLES2::MaterialDiffuseTextureCubeMapGLES2(const std::string &sceneId,
								       ShaderDiffuseTextureCubeMapGLES2 *shader,
								       ILoggerFactory *loggerFactory)
    : MaterialDiffuseTextureCubeMap(sceneId, shader, loggerFactory)
{
    logger = loggerFactory->createLogger("glImplementation::MaterialDiffuseTextureGLES2: ");
}
IRenderable *MaterialDiffuseTextureCubeMapGLES2::getRenderable()
{
    return this;
}
void MaterialDiffuseTextureCubeMapGLES2::setDiffuseTextureId(CL_GLuint diffuseTextureUniformId)
{
    this->diffuseTextureUniformId = diffuseTextureUniformId;
}
bool MaterialDiffuseTextureCubeMapGLES2::initialize()
{
    IRenderable *textureRenderable = this->diffuseTexture->getRenderable();
    assert(textureRenderable != nullptr);
    textureRenderable->initialize();
    logger->log(LOG_INFO, "material:" + sceneId + " initialized");
    return true;
}
void MaterialDiffuseTextureCubeMapGLES2::draw()
{
    // Bind our diffuse texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    this->diffuseTexture->getRenderable()->draw();
    glUniform1i(diffuseTextureUniformId, 0);
}
void MaterialDiffuseTextureCubeMapGLES2::deinitialize()
{
    this->diffuseTexture->getRenderable()->deinitialize();
}
}