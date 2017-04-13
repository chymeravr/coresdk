#include <glImplementation/ui/opengles2/MaterialTextGLES2.h>

namespace cl
{
MaterialTextGLES2::MaterialTextGLES2(const std::string &sceneId, ShaderTextGLES2 *shader, TextureTextGLES2 *texture, ILoggerFactory *loggerFactory)
    : MaterialText(sceneId, shader, texture, loggerFactory)
{
    logger = loggerFactory->createLogger("glImplementation::MaterialTextGLES2: ");
}
IRenderable *MaterialTextGLES2::getRenderable()
{
    return this;
}
void MaterialTextGLES2::setTextTextureId(CL_GLuint textTextureUniformId)
{
    this->textTextureUniformId = textTextureUniformId;
}
void MaterialTextGLES2::setColorId(CL_GLuint colorUniformId)
{
    this->colorUniformId = colorUniformId;
}
bool MaterialTextGLES2::initialize()
{
    IRenderable *textureRenderable = this->textureText->getRenderable();
    assert(textureRenderable != nullptr);
    textureRenderable->initialize();
    logger->log(LOG_INFO, "material:" + sceneId + " initialized");
    return true;
}
void MaterialTextGLES2::draw()
{
    glUniform4fv(colorUniformId, 1, &color[0]);
    glActiveTexture(GL_TEXTURE0);
    this->textureText->getRenderable()->draw();
    glUniform1i(textTextureUniformId, 0);
}
void MaterialTextGLES2::deinitialize()
{
    this->textureText->getRenderable()->deinitialize();
}
}