#ifndef COREENGINE_SHADERDIFFUSETEXTURE_H
#define COREENGINE_SHADERDIFFUSETEXTURE_H

#include <coreEngine/renderObjects/Shader.h>

namespace cl{
    class ShaderDiffuseTexture : public Shader{
    public:
        ShaderDiffuseTexture(const std::string &sceneId, IRelationStore *relationStore);
    };
}

#endif //COREENGINE_SHADERDIFFUSETEXTURE_H