#ifndef GLIMPLEMENTATION_TEXTUREGL_H
#define GLIMPLEMENTATION_TEXTUREGL_H

#include <coreEngine/renderObjects/Texture.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <conf/GLTypes.h>

namespace cl{
    class TextureGL : public Texture, public IRenderable{
    public:
        TextureGL(const std::string &sceneId); 
        IRenderable *getRenderable();
        bool initialize();
        void draw();
        void deinitialize();

        CL_GLuint getTextureId();
    };
}

#endif //GLIMPLEMENTATION_TEXTUREGL_H