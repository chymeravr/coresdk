#ifndef GLIMPLEMENTATION_MATERIALUNIFORMCOLORGLES3_H
#define GLIMPLEMENTATION_MATERIALUNIFORMCOLORGLES3_H

#include <coreEngine/renderObjects/MaterialUniformColor.h>
#include <glImplementation/renderObjects/opengles3/ShaderUniformColorGLES3.h>

namespace cl{
	class MaterialUniformColorGLES3 : public MaterialUniformColor, public IRenderable{
	public:
		MaterialUniformColorGLES3(const std::string &sceneId, ShaderUniformColorGLES3 *shader, ILoggerFactory *loggerFactory, CL_Vec4 &color);
		IRenderable *getRenderable();
		void setColorUniformId(CL_GLuint colorUniformId);//To be set by respective shader
		bool initialize();
		void draw();
		void deinitialize();
	private:
		std::unique_ptr<ILogger> logger;
		CL_GLuint colorUniformId;
	};
}

#endif //GLIMPLEMENTATION_MATERIALUNIFORMCOLORGLES3_H