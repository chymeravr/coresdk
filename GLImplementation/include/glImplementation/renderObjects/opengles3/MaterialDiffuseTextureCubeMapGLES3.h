#ifndef GLIMPLEMENTATION_MATERIALDIFFUSETEXTURECUBEMAPGLES3_H
#define GLIMPLEMENTATION_MATERIALDIFFUSETEXTURECUBEMAPGLES3_H

#include <coreEngine/renderObjects/MaterialDiffuseTextureCubeMap.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <conf/GLTypes.h>
#include <glImplementation/renderObjects/opengles3/ShaderDiffuseTextureCubeMapGLES3.h>

namespace cl{
	class MaterialDiffuseTextureCubeMapGLES3 : public MaterialDiffuseTextureCubeMap, public IRenderable{
	public:
		MaterialDiffuseTextureCubeMapGLES3(const std::string &sceneId, ShaderDiffuseTextureCubeMapGLES3 *shader, ILoggerFactory *loggerFactory);
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

#endif //GLIMPLEMENTATION_MATERIALDIFFUSETEXTURECUBEMAPGLES3_H