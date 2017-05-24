#ifndef GLIMPLEMENTATION_DIFFUSETEXTURECUBEMAPGLFACTORY_H
#define GLIMPLEMENTATION_DIFFUSETEXTURECUBEMAPGLFACTORY_H

#include <coreEngine/factory/IDiffuseTextureCubeMapFactory.h>
#include <glImplementation/renderObjects/MaterialDiffuseTextureCubeMapGL.h>
#include <glImplementation/renderObjects/ShaderDiffuseTextureCubeMapGL.h>
#include <glImplementation/renderObjects/TextureCubeMapGL.h>

namespace cl
{
	class DiffuseTextureCubeMapGLFactory : public IDiffuseTextureCubeMapFactory
	{
	public:
		DiffuseTextureCubeMapGLFactory(ILoggerFactory *loggerFactory)
		{
			this->loggerFactory = loggerFactory;
		}
		std::unique_ptr<ShaderDiffuseTextureCubeMap> createShader(const std::string &sceneId, Scene *scene)
		{
			return std::unique_ptr<ShaderDiffuseTextureCubeMap>(new ShaderDiffuseTextureCubeMapGL(sceneId, loggerFactory, scene));
		}
		std::unique_ptr<MaterialDiffuseTextureCubeMap> createMaterial(const std::string &sceneId, ShaderDiffuseTextureCubeMap *shader)
		{
			return std::unique_ptr<MaterialDiffuseTextureCubeMap>(new MaterialDiffuseTextureCubeMapGL(sceneId, (ShaderDiffuseTextureCubeMapGL *)shader, loggerFactory));
		}
		std::unique_ptr<TextureCubeMap> createTexture(const std::string &sceneId)
		{
			return std::unique_ptr<TextureCubeMap>(new TextureCubeMapGL(sceneId, loggerFactory));
		}

	private:
		ILoggerFactory *loggerFactory;
	};
}

#endif //GLIMPLEMENTATION_DIFFUSETEXTURECUBEMAPGLFACTORY_H
