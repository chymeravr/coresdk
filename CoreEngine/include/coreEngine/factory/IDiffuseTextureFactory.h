#ifndef COREENGINE_IDIFFUSETEXTUREFACTORY_H
#define COREENGINE_IDIFFUSETEXTUREFACTORY_H

#include <memory>
#include <coreEngine/renderObjects/ShaderDiffuseTexture.h>
#include <coreEngine/renderObjects/MaterialDiffuseTexture.h>
#include <coreEngine/renderObjects/Texture.h>

namespace cl{
	class IDiffuseTextureFactory{
	public:
		virtual ~IDiffuseTextureFactory(){}
		virtual std::unique_ptr<ShaderDiffuseTexture> createShader(const std::string &sceneId, Scene *scene) = 0;
		virtual std::unique_ptr<MaterialDiffuseTexture> createMaterial(const std::string &sceneId, ShaderDiffuseTexture *shader) = 0;
		virtual std::unique_ptr<Texture> createTexture(const std::string &sceneId) = 0;
	};
}

#endif //COREENGINE_IDIFFUSETEXTUREFACTORY_H