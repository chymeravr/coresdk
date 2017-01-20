#ifndef COREENGINE_IDIFFUSETEXTUREFACTORYCUBEMAP_H
#define COREENGINE_IDIFFUSETEXTUREFACTORYCUBEMAP_H

#include <memory>
#include <coreEngine/renderObjects/ShaderDiffuseTextureCubeMap.h>
#include <coreEngine/renderObjects/MaterialDiffuseTextureCubeMap.h>
#include <coreEngine/renderObjects/TextureCubeMap.h>

namespace cl
{
	class IDiffuseTextureCubeMapFactory
	{
	public:
		virtual ~IDiffuseTextureCubeMapFactory() {}
		virtual std::unique_ptr<ShaderDiffuseTextureCubeMap> createShader(const std::string &sceneId, Scene *scene) = 0;
		virtual std::unique_ptr<MaterialDiffuseTextureCubeMap> createMaterial(const std::string &sceneId, ShaderDiffuseTextureCubeMap *shader) = 0;
		virtual std::unique_ptr<TextureCubeMap> createTexture(const std::string &sceneId) = 0;
	};
}

#endif //COREENGINE_IDIFFUSETEXTUREFACTORYCUBEMAP_H