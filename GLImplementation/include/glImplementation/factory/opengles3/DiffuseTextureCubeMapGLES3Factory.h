#ifndef GLIMPLEMENTATION_DIFFUSETEXTURECUBEMAPGLES3FACTORY_H
#define GLIMPLEMENTATION_DIFFUSETEXTURECUBEMAPGLES3FACTORY_H

#include <coreEngine/factory/IDiffuseTextureCubeMapFactory.h>
#include <glImplementation/renderObjects/opengles3/MaterialDiffuseTextureCubeMapGLES3.h>
#include <glImplementation/renderObjects/opengles3/ShaderDiffuseTextureCubeMapGLES3.h>
#include <glImplementation/renderObjects/TextureCubeMapGL.h>

namespace cl
{
	class DiffuseTextureCubeMapGLES3Factory : public IDiffuseTextureCubeMapFactory
	{
	public:
		DiffuseTextureCubeMapGLES3Factory(ILoggerFactory *loggerFactory)
		{
			this->loggerFactory = loggerFactory;
		}
		std::unique_ptr<ShaderDiffuseTextureCubeMap> createShader(const std::string &sceneId, Scene *scene)
		{
			return std::unique_ptr<ShaderDiffuseTextureCubeMap>(new ShaderDiffuseTextureCubeMapGLES3(sceneId, loggerFactory, scene));
		}
		std::unique_ptr<MaterialDiffuseTextureCubeMap> createMaterial(const std::string &sceneId, ShaderDiffuseTextureCubeMap *shader)
		{
			return std::unique_ptr<MaterialDiffuseTextureCubeMap>(new MaterialDiffuseTextureCubeMapGLES3(sceneId, (ShaderDiffuseTextureCubeMapGLES3 *)shader,
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

#endif //GLIMPLEMENTATION_DIFFUSETEXTURECUBEMAPGLES3FACTORY_H
