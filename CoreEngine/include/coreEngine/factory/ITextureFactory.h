#ifndef COREENGINE_ITEXTUREFACTORY_H
#define COREENGINE_ITEXTUREFACTORY_H

#include <memory>
#include <coreEngine/renderObjects/Texture.h>

namespace cl{
    class ITextureFactory{
    public:
        virtual std::unique_ptr<Texture> create(const std::string &sceneId) = 0;
    };
}

#endif //COREENGINE_ITEXTUREFACTORY_H