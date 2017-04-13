#ifndef GLIMPLEMENTATION_TEXTURETEXTGLES2_H
#define GLIMPLEMENTATION_TEXTURETEXTGLES2_H

#include <coreEngine/ui/TextureText.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <conf/GLTypes.h>

namespace cl
{
class TextureTextGLES2 : public TextureText, IRenderable
{
  public:
    TextureTextGLES2(const std::string &sceneId, ILoggerFactory *loggerFactory, const unsigned int &width, const unsigned int &height, std::unique_ptr<unsigned char> data);
    IRenderable *getRenderable();
    bool initialize();
    void draw();
    void deinitialize();

  private:
    CL_GLuint textureId;
    std::unique_ptr<ILogger> logger = nullptr;
};
}

#endif //GLIMPLEMENTATION_TEXTURETEXTGL_H