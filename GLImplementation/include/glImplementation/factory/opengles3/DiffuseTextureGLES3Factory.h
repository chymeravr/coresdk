#ifndef GLIMPLEMENTATION_DIFFUSETEXTUREGLES3FACTORY_H
#define GLIMPLEMENTATION_DIFFUSETEXTUREGLES3FACTORY_H

#include <coreEngine/factory/IDiffuseTextureFactory.h>
#include <glImplementation/renderObjects/opengles3/MaterialDiffuseTextureGLES3.h>
#include <glImplementation/renderObjects/opengles3/ShaderDiffuseTextureGLES3.h>
#include <glImplementation/renderObjects/TextureGL.h>

namespace cl
{
	class DiffuseTextureGLES3Factory : public IDiffuseTextureFactory
	{
	public:
		DiffuseTextureGLES3Factory(ILoggerFactory *loggerFactory)
		{
			this->loggerFactory = loggerFactory;
		}
		std::unique_ptr<ShaderDiffuseTexture> createShader(const std::string &sceneId, Scene *scene)
		{
			return std::unique_ptr<ShaderDiffuseTexture>(new ShaderDiffuseTextureGLES3(sceneId, loggerFactory, scene));
		}
		std::unique_ptr<MaterialDiffuseTexture> createMaterial(const std::string &sceneId, ShaderDiffuseTexture *shader)
		{
			return std::unique_ptr<MaterialDiffuseTexture>(new MaterialDiffuseTextureGLES3(sceneId, (ShaderDiffuseTextureGLES3 *)shader, loggerFactory));
		}
		std::unique_ptr<Texture> createTexture(const std::string &sceneId)
		{
			return std::unique_ptr<Texture>(new TextureGL(sceneId, loggerFactory));
		}

	private:
		ILoggerFactory *loggerFactory;
	};
}

#endif //GLIMPLEMENTATION_DIFFUSETEXTUREGLES3FACTORY_H
