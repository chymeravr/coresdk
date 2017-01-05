#ifndef GLIMPLEMENTATION_MATERIALDIFFUSETEXTUREGLES3_H
#define GLIMPLEMENTATION_MATERIALDIFFUSETEXTUREGLES3_H

#include <coreEngine/renderObjects/MaterialDiffuseTexture.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <conf/GLTypes.h>
#include <glImplementation/renderObjects/opengles3/ShaderDiffuseTextureGLES3.h>

namespace cl{
	class MaterialDiffuseTextureGLES3 : public MaterialDiffuseTexture, public IRenderable{
	public:
		MaterialDiffuseTextureGLES3(const std::string &sceneId, ShaderDiffuseTextureGLES3 *shader, ILoggerFactory *loggerFactory);
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

#endif //GLIMPLEMENTATION_MATERIALDIFFUSETEXTUREGLES3_H