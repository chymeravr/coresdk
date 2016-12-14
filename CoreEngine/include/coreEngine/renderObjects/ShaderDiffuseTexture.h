#ifndef COREENGINE_SHADERDIFFUSETEXTURE_H
#define COREENGINE_SHADERDIFFUSETEXTURE_H

#include <coreEngine/renderObjects/Shader.h>

namespace cl{
    class ShaderDiffuseTexture : public Shader{
    public:
        virtual ~ShaderDiffuseTexture(){}
        ShaderDiffuseTexture(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene) : Shader(sceneId, loggerFactory, scene){}
    };
}

#endif //COREENGINE_SHADERDIFFUSETEXTURE_H