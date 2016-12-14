#ifndef COREENGINE_IMATERIALDIFFUSETEXTUREFACTORY_H
#define COREENGINE_IMATERIALDIFFUSETEXTUREFACTORY_H

#include <coreEngine/renderObjects/MaterialDiffuseTexture.h>

namespace cl{
    class IMaterialDiffuseTextureFactory{
    public:
        virtual std::unique_ptr<MaterialDiffuseTexture> create(const std::string &sceneId, ShaderDiffuseTexture *shader, ILoggerFactory *loggerFactory) = 0;
    };
}

#endif //COREENGINE_IMATERIALDIFFUSETEXTUREFACTORY_H