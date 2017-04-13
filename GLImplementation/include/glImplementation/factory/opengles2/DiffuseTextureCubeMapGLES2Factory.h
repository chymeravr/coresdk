#ifndef GLIMPLEMENTATION_DIFFUSETEXTURECUBEMAPGLES2FACTORY_H
#define GLIMPLEMENTATION_DIFFUSETEXTURECUBEMAPGLES2FACTORY_H

#include <coreEngine/factory/IDiffuseTextureCubeMapFactory.h>
#include <glImplementation/renderObjects/opengles2/MaterialDiffuseTextureCubeMapGLES2.h>
#include <glImplementation/renderObjects/opengles2/ShaderDiffuseTextureCubeMapGLES2.h>
#include <glImplementation/renderObjects/TextureCubeMapGL.h>

namespace cl
{
class DiffuseTextureCubeMapGLES2Factory : public IDiffuseTextureCubeMapFactory
{
  public:
    DiffuseTextureCubeMapGLES2Factory(ILoggerFactory *loggerFactory)
    {
	this->loggerFactory = loggerFactory;
    }
    std::unique_ptr<ShaderDiffuseTextureCubeMap> createShader(const std::string &sceneId, Scene *scene)
    {
	return std::unique_ptr<ShaderDiffuseTextureCubeMap>(new ShaderDiffuseTextureCubeMapGLES2(sceneId, loggerFactory, scene));
    }
    std::unique_ptr<MaterialDiffuseTextureCubeMap> createMaterial(const std::string &sceneId, ShaderDiffuseTextureCubeMap *shader)
    {
	return std::unique_ptr<MaterialDiffuseTextureCubeMap>(new MaterialDiffuseTextureCubeMapGLES2(sceneId, (ShaderDiffuseTextureCubeMapGLES2 *)shader,
												     loggerFactory));
    }
    std::unique_ptr<TextureCubeMap> createTexture(const std::string &sceneId)
    {
	return std::unique_ptr<TextureCubeMap>(new TextureCubeMapGL(sceneId, loggerFactory));
    }

  private:
    ILoggerFactory *loggerFactory;
};
}

#endif //GLIMPLEMENTATION_DIFFUSETEXTURECUBEMAPGLES2FACTORY_H
