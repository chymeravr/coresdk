#include <glImplementation/ui/opengles3/TextureTextGLES3.h>

namespace cl
{
TextureTextGLES3::TextureTextGLES3(const std::string &sceneId, ILoggerFactory *loggerFactory, const unsigned int &width,
                                   const unsigned int &height, std::unique_ptr<unsigned char> data) : TextureText(sceneId, loggerFactory, width, height,
                                                                                                                  std::move(data))
{
    logger = loggerFactory->createLogger("glImplementation::TextureTextGL: ");
}
IRenderable *TextureTextGLES3::getRenderable()
{
    return this;
}
bool TextureTextGLES3::initialize()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, unpackAlignmentValue);
    glGenTextures(1, &textureId);
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Give the image to OpenGL
    assert(data != nullptr);

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
void TextureTextGLES3::draw()
{
    glBindTexture(GL_TEXTURE_2D, textureId);
}
void TextureTextGLES3::deinitialize()
{
    glDeleteTextures(1, &textureId);
}
}