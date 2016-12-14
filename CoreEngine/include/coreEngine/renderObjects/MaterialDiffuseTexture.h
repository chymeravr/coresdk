#ifndef COREENGINE_MATERIALDIFFUSETEXTURE_H
#define COREENGINE_MATERIALDIFFUSETEXTURE_H

#include <coreEngine/renderObjects/Material.h>
#include <coreEngine/renderObjects/Texture.h>
#include <coreEngine/renderObjects/ShaderDiffuseTexture.h>

namespace cl{
    class MaterialDiffuseTexture : public Material{
    public:
        virtual ~MaterialDiffuseTexture(){}
        MaterialDiffuseTexture(const std::string &sceneId, ShaderDiffuseTexture *shader, ILoggerFactory *loggerFactory) : Material(sceneId, shader, loggerFactory){}
        void setDiffuseTexture(Texture *texture){
            this->diffuseTexture = texture;
        }
        Texture *getDiffuseTexture(){
            return this->diffuseTexture;
        }
    protected:
        Texture *diffuseTexture;
    };
}

#endif //COREENGINE_MATERIALDIFFUSETEXTURE_H