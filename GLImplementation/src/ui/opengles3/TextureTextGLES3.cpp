#include <glImplementation/ui/TextureTextGL.h>

namespace cl
{
TextureTextGL::TextureTextGL(const std::string &sceneId, ILoggerFactory *loggerFactory, const unsigned int &width,
                             const unsigned int &height, std::unique_ptr<unsigned char> data)
    : TextureText(sceneId, loggerFactory, width, height, std::move(data))
{
    logger = loggerFactory->createLogger("glImplementation::TextureTextGLES3: ");
}
IRenderable *TextureTextGL::getRenderable()
{
    return this;
}
bool TextureTextGL::initialize()
{

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &textureId);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Give the image to OpenGL
    assert(data != nullptr);
    // GL_RED tho supported on opengles3 it does not render text - use GL_LUMINANCE instead that works - 
    // does this work cross platform
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_LUMINANCE,
        width,
        height,
        0,
        GL_LUMINANCE,
        GL_UNSIGNED_BYTE,
        data.get());
    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    this->clearTextureData();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); //setting back to default value
    logger->log(LOG_INFO, "texture:" + sceneId + " initialized");

    return true;
}
void TextureTextGL::draw()
{
    // todo: change all related gl states for rendering here
    glBindTexture(GL_TEXTURE_2D, textureId);
}
void TextureTextGL::deinitialize()
{
    glDeleteTextures(1, &textureId);
}
}