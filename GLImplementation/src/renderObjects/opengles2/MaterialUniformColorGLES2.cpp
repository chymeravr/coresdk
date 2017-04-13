#include <glImplementation/renderObjects/opengles2/MaterialUniformColorGLES2.h>

namespace cl
{
MaterialUniformColorGLES2::MaterialUniformColorGLES2(const std::string &sceneId, ShaderUniformColorGLES2 *shader, ILoggerFactory *loggerFactory, CL_Vec4 &color)
    : MaterialUniformColor(sceneId, shader, loggerFactory, color)
{
    logger = loggerFactory->createLogger("glImplementation::MaterialUniformColorGLES2: ");
}
IRenderable *MaterialUniformColorGLES2::getRenderable()
{
    return this;
}
void MaterialUniformColorGLES2::setColorUniformId(CL_GLuint colorUniformId)
{
    this->colorUniformId = colorUniformId;
}
bool MaterialUniformColorGLES2::initialize()
{
    logger->log(LOG_INFO, "material:" + sceneId + " initialized");
    return true;
}
void MaterialUniformColorGLES2::draw()
{
    glUniform4fv(colorUniformId, 1, &color[0]);
}
void MaterialUniformColorGLES2::deinitialize()
{
}
}