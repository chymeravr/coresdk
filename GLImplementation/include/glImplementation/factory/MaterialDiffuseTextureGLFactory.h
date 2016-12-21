#ifndef GLIMPLEMENTATION_MATERIALDIFFUSETEXTUREGLFACTORY_H
#define GLIMPLEMENTATION_MATERIALDIFFUSETEXTUREGLFACTORY_H

#include <coreEngine/factory/IMaterialDiffuseTextureFactory.h>
#include <glImplementation/renderObjects/MaterialDiffuseTextureGL.h>

namespace cl{
    class MaterialDiffuseTextureGLFactory : public IMaterialDiffuseTextureFactory{
    public:
        MaterialDiffuseTextureGLFactory(ILoggerFactory *loggerFactory){
            this->loggerFactory = loggerFactory;
        }
        std::unique_ptr<MaterialDiffuseTexture> create(const std::string &sceneId, ShaderDiffuseTexture *shader){
            return std::unique_ptr<MaterialDiffuseTexture>(new MaterialDiffuseTextureGL(sceneId, (ShaderDiffuseTextureGL*)shader, loggerFactory));
        }
    private:
        ILoggerFactory *loggerFactory;
    };
}

#endif //GLIMPLEMENTATION_MATERIALDIFFUSETEXTUREGLFACTORY_H