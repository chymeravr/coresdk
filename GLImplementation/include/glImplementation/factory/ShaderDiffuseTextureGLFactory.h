#ifndef GLIMPLEMENTATION_SHADERDIFFUSETEXTUREGLFACTORY_H
#define GLIMPLEMENTATION_SHADERDIFFUSETEXTUREGLFACTORY_H

#include <coreEngine/factory/IShaderDiffuseTextureFactory.h>
#include <glImplementation/renderObjects/ShaderDiffuseTextureGL.h>

namespace cl{
    class ShaderDiffuseTextureGLFactory : public IShaderDiffuseTextureFactory{
    public:
        ShaderDiffuseTextureGLFactory(ILoggerFactory *loggerFactory){
            this->loggerFactory = loggerFactory;
        }
        std::unique_ptr<ShaderDiffuseTexture> create(const std::string &sceneId, Scene *scene){
            return std::unique_ptr<ShaderDiffuseTexture>(new ShaderDiffuseTextureGL(sceneId, loggerFactory, scene));
        }
    private:
        ILoggerFactory *loggerFactory;
    };
}

#endif //GLIMPLEMENTATION_SHADERDIFFUSETEXTUREGLFACTORY_H