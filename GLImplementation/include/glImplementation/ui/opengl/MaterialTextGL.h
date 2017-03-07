#ifndef GLIMPLEMENTATION_MATERIALTEXTGL_H
#define GLIMPLEMENTATION_MATERIALTEXTGL_H

#include <coreEngine/ui/MaterialText.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <glImplementation/ui/opengl/ShaderTextGL.h>
#include <glImplementation/ui/TextureTextGL.h>

namespace cl{
	class MaterialTextGL : public MaterialText, public IRenderable{
	public:
		MaterialTextGL(const std::string &sceneId, ShaderTextGL *shader, TextureTextGL *texture, ILoggerFactory *loggerFactory);
		IRenderable *getRenderable();
		void setTextTextureId(CL_GLuint textTextureUniformId);//To be set by respective shader
		void setColorId(CL_GLuint colorUniformId);
		bool initialize();
		void draw();
		void deinitialize();

	private:
		CL_GLuint textTextureUniformId;
		CL_GLuint colorUniformId;
		std::unique_ptr<ILogger> logger = nullptr;
	};
}

#endif //GLIMPLEMENTATION_MATERIALTEXTGL_H