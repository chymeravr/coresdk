#include <glImplementation/renderObjects/opengles2/MaterialDiffuseTextureGLES2.h>
#include <glImplementation/renderObjects/TextureGL.h>

namespace cl
{
MaterialDiffuseTextureGLES2::MaterialDiffuseTextureGLES2(const std::string &sceneId, ShaderDiffuseTextureGLES2 *shader, ILoggerFactory *loggerFactory)
    : MaterialDiffuseTexture(sceneId, shader, loggerFactory)
{
    logger = loggerFactory->createLogger("glImplementation::MaterialDiffuseTextureGLES2: ");
}
IRenderable *MaterialDiffuseTextureGLES2::getRenderable()
{
    return this;
}
void MaterialDiffuseTextureGLES2::setDiffuseTextureId(CL_GLuint diffuseTextureUniformId)
{
    this->diffuseTextureUniformId = diffuseTextureUniformId;
}
bool MaterialDiffuseTextureGLES2::initialize()
{
    IRenderable *textureRenderable = this->diffuseTexture->getRenderable();
    assert(textureRenderable != nullptr);
    textureRenderable->initialize();
    logger->log(LOG_INFO, "material:" + sceneId + " initialized");
    return true;
}
void MaterialDiffuseTextureGLES2::draw()
{
    // Bind our diffuse texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    this->diffuseTexture->getRenderable()->draw();
    glUniform1i(diffuseTextureUniformId, 0);
}
void MaterialDiffuseTextureGLES2::deinitialize()
{
    this->diffuseTexture->getRenderable()->deinitialize();
}
}