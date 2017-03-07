#ifndef GLIMPLEMENTATION_MATERIALTEXTGLES3_H
#define GLIMPLEMENTATION_MATERIALTEXTGLES3_H

#include <coreEngine/ui/MaterialText.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <glImplementation/ui/opengles3/ShaderTextGLES3.h>
#include <glImplementation/ui/TextureTextGL.h>

namespace cl{
	class MaterialTextGLES3 : public MaterialText, public IRenderable{
	public:
		MaterialTextGLES3(const std::string &sceneId, ShaderTextGLES3 *shader, TextureTextGL *texture, ILoggerFactory *loggerFactory);
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

#endif //GLIMPLEMENTATION_MATERIALTEXTGLES3_H