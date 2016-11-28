#ifndef ANDROIDSDK_TEXTUREFACTORY_H
#define ANDROIDSDK_TEXTUREFACTORY_H

#include<coreEngine/factory/ITextureFactory.h>

namespace cl{
    class TextureFactory : public ITextureFactory{
    public:
        std::unique_ptr<Object> create(std::string tag, std::unique_ptr<ITextureRenderer> textureRendererPtr){
            return std::unique_ptr<Object>(new Texture(std::move(textureRendererPtr), tag));
        }
        void destroy(Texture *texturePtr){
        }
    };
}

#endif //ANDROIDSDK_TEXTUREFACTORY_H