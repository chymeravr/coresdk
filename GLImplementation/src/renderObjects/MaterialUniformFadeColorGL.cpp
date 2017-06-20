#include <glImplementation/renderObjects/MaterialUniformFadeColorGL.h>

namespace cl
{
MaterialUniformFadeColorGL::MaterialUniformFadeColorGL(const std::string &sceneId,
                                                       ShaderUniformFadeColorGL *shader, ILoggerFactory *loggerFactory, CL_Vec4 &color)
    : MaterialUniformFadeColor(sceneId, shader, loggerFactory, color)
{
    logger = loggerFactory->createLogger("glImplementation::MaterialUniformFadeColorGL: ");
}
IRenderable *MaterialUniformFadeColorGL::getRenderable()
{
    return this;
}
void MaterialUniformFadeColorGL::setColorUniformId(CL_GLuint colorUniformId)
{
    this->colorUniformId = colorUniformId;
}
bool MaterialUniformFadeColorGL::initialize()
{
    logger->log(LOG_INFO, "material:" + sceneId + " initialized");
    return true;
}
void MaterialUniformFadeColorGL::draw()
{
    glUniform4fv(colorUniformId, 1, &color[0]);
}
void MaterialUniformFadeColorGL::deinitialize()
{
}
}