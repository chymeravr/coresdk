#ifndef COREENGINE_MATERIALDIFFUSETEXTURECUBEMAP_H
#define COREENGINE_MATERIALDIFFUSETEXTURECUBEMAP_H

#include <coreEngine/renderObjects/TextureCubeMap.h>
#include <coreEngine/renderObjects/Material.h>
#include <coreEngine/renderObjects/ShaderDiffuseTextureCubeMap.h>

namespace cl{
	class MaterialDiffuseTextureCubeMap : public Material {
	public:
		virtual ~MaterialDiffuseTextureCubeMap(){}
		MaterialDiffuseTextureCubeMap(const std::string &sceneId, ShaderDiffuseTextureCubeMap *shader, ILoggerFactory *loggerFactory) : Material(sceneId, shader, loggerFactory){}
		void setDiffuseTexture(Texture *texture){
			this->diffuseTexture = texture;
		}
		Texture *getDiffuseTexture(){
			return this->diffuseTexture;
		}
	protected:
		Texture *diffuseTexture;
	};
}

#endif //COREENGINE_MATERIALDIFFUSETEXTURECUBEMAP_H