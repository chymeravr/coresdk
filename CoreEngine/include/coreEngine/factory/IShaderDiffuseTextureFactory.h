#ifndef COREENGINE_ISHADERDIFFUSETEXTUREFACTORY_H
#define COREENGINE_ISHADERDIFFUSETEXTUREFACTORY_H

#include <memory>
#include <coreEngine/renderObjects/ShaderDiffuseTexture.h>

namespace cl{
    class IShaderDiffuseTextureFactory{
    public:
        virtual std::unique_ptr<ShaderDiffuseTexture> create(const std::string &sceneId, IRelationStore *relationStore) = 0;
    };
}

#endif //COREENGINE_ISHADERDIFFUSETEXTUREFACTORY_H