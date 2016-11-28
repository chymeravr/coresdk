
#ifndef ANDROIDSDK_ITEXTUREFACTORY_H
#define ANDROIDSDK_ITEXTUREFACTORY_H

#include<memory>
#include<coreEngine/model/Object.h>
#include<coreEngine/model/Texture.h>
#include<coreEngine/model/ITextureRenderer.h>

namespace cl{
    class ITextureFactory{
    public:
        virtual std::unique_ptr<Object> create(std::string tag, std::unique_ptr<ITextureRenderer> textureRendererPtr) = 0;
        virtual void destroy(Texture *texturePtr) = 0;
    };
}

#endif //ANDROIDSDK_ITEXTUREFACTORY_H