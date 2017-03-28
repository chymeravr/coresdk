#ifndef GLIMPLEMENTATION_DIFFUSETEXTUREGLES2FACTORY_H
#define GLIMPLEMENTATION_DIFFUSETEXTUREGLES2FACTORY_H

#include <coreEngine/factory/IDiffuseTextureFactory.h>
#include <glImplementation/renderObjects/opengles2/MaterialDiffuseTextureGLES2.h>
#include <glImplementation/renderObjects/opengles2/ShaderDiffuseTextureGLES2.h>
#include <glImplementation/renderObjects/TextureGL.h>

namespace cl
{
class DiffuseTextureGLES2Factory : public IDiffuseTextureFactory
{
  public:
    DiffuseTextureGLES2Factory(ILoggerFactory *loggerFactory)
    {
        this->loggerFactory = loggerFactory;
    }
    std::unique_ptr<ShaderDiffuseTexture> createShader(const std::string &sceneId, Scene *scene)
    {
        return std::unique_ptr<ShaderDiffuseTexture>(new ShaderDiffuseTextureGLES2(sceneId, loggerFactory, scene));
    }
    std::unique_ptr<MaterialDiffuseTexture> createMaterial(const std::string &sceneId, ShaderDiffuseTexture *shader)
    {
        return std::unique_ptr<MaterialDiffuseTexture>(new MaterialDiffuseTextureGLES2(sceneId, (ShaderDiffuseTextureGLES2 *)shader, loggerFactory));
    }
    std::unique_ptr<Texture> createTexture(const std::string &sceneId)
    {
        return std::unique_ptr<Texture>(new TextureGL(sceneId, loggerFactory));
    }

  private:
    ILoggerFactory *loggerFactory;
};
}

#endif //GLIMPLEMENTATION_DIFFUSETEXTUREGLES2FACTORY_H
