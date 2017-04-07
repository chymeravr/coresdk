#include <glImplementation/ui/opengles3/MaterialTextGLES3.h>

namespace cl
{
MaterialTextGLES3::MaterialTextGLES3(const std::string &sceneId, ShaderTextGLES3 *shader, TextureTextGLES3 *texture, ILoggerFactory *loggerFactory)
    : MaterialText(sceneId, shader, texture, loggerFactory)
{
    logger = loggerFactory->createLogger("glImplementation::MaterialTextGLES3: ");
}
IRenderable *MaterialTextGLES3::getRenderable()
{
    return this;
}
void MaterialTextGLES3::setTextTextureId(CL_GLuint textTextureUniformId)
{
    this->textTextureUniformId = textTextureUniformId;
}
void MaterialTextGLES3::setColorId(CL_GLuint colorUniformId)
{
    this->colorUniformId = colorUniformId;
}
bool MaterialTextGLES3::initialize()
{
    IRenderable *textureRenderable = this->textureText->getRenderable();
    assert(textureRenderable != nullptr);
    textureRenderable->initialize();
    logger->log(LOG_INFO, "material:" + sceneId + " initialized");
    return true;
}
void MaterialTextGLES3::draw()
{
    glUniform4fv(colorUniformId, 1, &color[0]);
    glActiveTexture(GL_TEXTURE0);
    this->textureText->getRenderable()->draw();
    glUniform1i(textTextureUniformId, 0);
}
void MaterialTextGLES3::deinitialize()
{
    this->textureText->getRenderable()->deinitialize();
}
}