#ifndef GLIMPLEMENTATION_TEXTUREGLFACTORY_H
#define GLIMPLEMENTATION_TEXTUREGLFACTORY_H

#include <coreEngine/factory/ITextureFactory.h>
#include <glImplementation/renderObjects/TextureGL.h>

namespace cl{
    class TextureGLFactory : public ITextureFactory{
    public:
        TextureGLFactory(ILoggerFactory *loggerFactory){
            this->loggerFactory = loggerFactory;
        }
        std::unique_ptr<Texture> create(const std::string &sceneId){
            return std::unique_ptr<Texture>(new TextureGL(sceneId, loggerFactory));
        }
    private:
        ILoggerFactory *loggerFactory;
    };
}

#endif //GLIMPLEMENTATION_TEXTUREGLFACTORY_H