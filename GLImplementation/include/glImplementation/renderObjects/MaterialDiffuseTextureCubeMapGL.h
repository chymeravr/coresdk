#ifndef GLIMPLEMENTATION_MATERIALDIFFUSETEXTURECUBEMAPGL_H
#define GLIMPLEMENTATION_MATERIALDIFFUSETEXTURECUBEMAPGL_H

#include <coreEngine/renderObjects/MaterialDiffuseTextureCubeMap.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <conf/GLTypes.h>
#include <glImplementation/renderObjects/ShaderDiffuseTextureCubeMapGL.h>

namespace cl{
	class MaterialDiffuseTextureCubeMapGL : public MaterialDiffuseTextureCubeMap, public IRenderable{
	public:
		MaterialDiffuseTextureCubeMapGL(const std::string &sceneId, ShaderDiffuseTextureCubeMapGL *shader, ILoggerFactory *loggerFactory);
		IRenderable *getRenderable();
		void setDiffuseTextureId(CL_GLuint diffuseTextureUniformId);//To be set by respective shader
		bool initialize();
		void draw();
		void deinitialize();
	private:
		std::unique_ptr<ILogger> logger;
		CL_GLuint diffuseTextureUniformId;
	};
}

#endif //GLIMPLEMENTATION_MATERIALDIFFUSETEXTURECUBEMAPGL_H