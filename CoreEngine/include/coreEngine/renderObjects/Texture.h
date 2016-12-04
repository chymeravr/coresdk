#ifndef COREENGINE_TEXTURE_H
#define COREENGINE_TEXTURE_H

#include <memory>
#include <coreEngine/renderObjects/IScenable.h>
#include <coreEngine/renderObjects/IRenderable.h>

namespace cl{
    class Texture : public IScenable{
    public:
        Texture(const std::string &sceneId);
        virtual IRenderable *getRenderable() = 0;
        std::string getSceneId();
        virtual void setTextureData(std::unique_ptr<unsigned char> dataPtr);
        virtual const unsigned char  *getTextureData();
        virtual void clearTextureData();
        virtual void setTextureDataSize(const unsigned int &size);
        virtual const unsigned int &getTextureDataSize();
    };
}

#endif //COREENGINE_TEXTURE_H