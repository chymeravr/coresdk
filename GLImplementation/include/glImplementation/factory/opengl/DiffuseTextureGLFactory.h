#ifndef GLIMPLEMENTATION_DIFFUSETEXTUREGLFACTORY_H
#define GLIMPLEMENTATION_DIFFUSETEXTUREGLFACTORY_H

#include <coreEngine/factory/IDiffuseTextureFactory.h>
#include <glImplementation/renderObjects/opengl/MaterialDiffuseTextureGL.h>
#include <glImplementation/renderObjects/opengl/ShaderDiffuseTextureGL.h>
#include <glImplementation/renderObjects/TextureGL.h>

namespace cl
{
	class DiffuseTextureGLFactory : public IDiffuseTextureFactory
	{
	public:
		DiffuseTextureGLFactory(ILoggerFactory *loggerFactory)
		{
			this->loggerFactory = loggerFactory;
		}
		std::unique_ptr<ShaderDiffuseTexture> createShader(const std::string &sceneId, Scene *scene)
		{
			return std::unique_ptr<ShaderDiffuseTexture>(new ShaderDiffuseTextureGL(sceneId, loggerFactory, scene));
		}
		std::unique_ptr<MaterialDiffuseTexture> createMaterial(const std::string &sceneId, ShaderDiffuseTexture *shader)
		{
			return std::unique_ptr<MaterialDiffuseTexture>(new MaterialDiffuseTextureGL(sceneId, (ShaderDiffuseTextureGL *)shader, loggerFactory));
		}
		std::unique_ptr<Texture> createTexture(const std::string &sceneId)
		{
			return std::unique_ptr<Texture>(new TextureGL(sceneId, loggerFactory));
		}

	private:
		ILoggerFactory *loggerFactory;
	};
}

#endif //GLIMPLEMENTATION_DIFFUSETEXTUREGLFACTORY_H
