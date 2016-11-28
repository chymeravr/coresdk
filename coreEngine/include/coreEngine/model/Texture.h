#ifndef ANDROIDSDK_TEXTURE_H
#define ANDROIDSDK_TEXTURE_H

#include <vector>
#include <coreEngine/model/Object.h>
#include <coreEngine/model/ITextureRenderer.h>
#include <coreEngine/model/Material.h>

namespace cl{
    class Texture : public Object{
    private:
        std::unique_ptr<unsigned char> dataPtr;
        unsigned int size;
        std::unique_ptr<ITextureRenderer> textureRendererPtr;
    public:
        Texture(std::unique_ptr<ITextureRenderer> textureRendererPtr, std::string tag) : Object("texture", tag){
            dataPtr = nullptr;
            this->textureRendererPtr = std::move(textureRendererPtr);
        }

        void setTextureData(std::unique_ptr<unsigned char> dataPtr){
            this->dataPtr = std::move(dataPtr);
        }

        const unsigned char  *getTextureData(){
            return this->dataPtr.get();
        }

        void clearTextureData(){
            dataPtr = nullptr;
        }

        void setTextureDataSize(const unsigned int &size){
            this->size = size;
        }

        const unsigned int &getTextureDataSize(){
            return this->size;
        }

    };
}

#endif //ANDROIDSDK_TEXTURE_H