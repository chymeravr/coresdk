#ifndef COREENGINE_MATERIALDIFFUSETEXTURE_H
#define COREENGINE_MATERIALDIFFUSETEXTURE_H

#include <coreEngine/renderObjects/Material.h>
#include <coreEngine/renderObjects/Texture.h>

namespace cl{
    class MaterialDiffuseTexture : public Material{
    public:
        MaterialDiffuseTexture(const std::string &sceneId, IRelationStore *relationStore, Shader *shader);
        void setDiffuseTexture(Texture *texture);
        Texture *getDiffuseTexture();
    };
}

#endif //COREENGINE_MATERIALDIFFUSETEXTURE_H