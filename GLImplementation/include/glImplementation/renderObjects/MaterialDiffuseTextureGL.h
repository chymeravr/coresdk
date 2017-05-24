#ifndef GLIMPLEMENTATION_MATERIALDIFFUSETEXTUREGL_H
#define GLIMPLEMENTATION_MATERIALDIFFUSETEXTUREGL_H

#include <coreEngine/renderObjects/MaterialDiffuseTexture.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <conf/GLTypes.h>
#include <glImplementation/renderObjects/ShaderDiffuseTextureGL.h>

namespace cl{
	class MaterialDiffuseTextureGL : public MaterialDiffuseTexture, public IRenderable{
	public:
		MaterialDiffuseTextureGL(const std::string &sceneId, ShaderDiffuseTextureGL *shader, ILoggerFactory *loggerFactory);
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

#endif //GLIMPLEMENTATION_MATERIALDIFFUSETEXTUREGL_H