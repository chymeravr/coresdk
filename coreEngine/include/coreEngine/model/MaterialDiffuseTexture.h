#ifndef ANDROIDSDK_MATERIALDIFFUSETEXTURE_H
#define ANDROIDSDK_MATERIALDIFFUSETEXTURE_H

#include <coreEngine/model/Material.h>
#include <coreEngine/model/Texture.h>
#include <coreEngine/model/IMaterialRenderer.h>

namespace cl{
    class MaterialDiffuseTexture : public Material{
    private:
        Texture *diffuseTexturePtr;
    public:
        MaterialDiffuseTexture(std::unique_ptr<IMaterialRenderer> materialRendererPtr,
            const std::string &tag) : Material(std::move(materialRendererPtr), tag){
            diffuseTexturePtr = nullptr;
        }
        void setDiffuseTexture(Texture *diffuseTexturePtr){
            this->diffuseTexturePtr = diffuseTexturePtr;
        }
        Texture *getDiffuseTexture(){
            return diffuseTexturePtr;
        }
    };
}

#endif //ANDROIDSDK_MATERIALDIFFUSETEXTURE_H
