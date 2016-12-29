#ifndef COREENGINE_SHADERDIFFUSETEXTURECUBEMAP_H
#define COREENGINE_SHADERDIFFUSETEXTURECUBEMAP_H

#include <coreEngine/renderObjects/Shader.h>

namespace cl{
    class ShaderDiffuseTextureCubeMap : public Shader{
    public:
        virtual ~ShaderDiffuseTextureCubeMap(){}
        ShaderDiffuseTextureCubeMap(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene) : Shader(sceneId, loggerFactory, scene){}
    };
}

#endif //COREENGINE_SHADERDIFFUSETEXTURECUBEMAP_H