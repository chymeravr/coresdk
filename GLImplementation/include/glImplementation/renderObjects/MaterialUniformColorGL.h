#ifndef GLIMPLEMENTATION_MATERIALUNIFORMCOLORGL_H
#define GLIMPLEMENTATION_MATERIALUNIFORMCOLORGL_H

#include <coreEngine/renderObjects/MaterialUniformColor.h>
#include <glImplementation/renderObjects/ShaderUniformColorGL.h>

namespace cl{
	class MaterialUniformColorGL : public MaterialUniformColor, public IRenderable{
	public:
		MaterialUniformColorGL(const std::string &sceneId, ShaderUniformColorGL *shader, ILoggerFactory *loggerFactory, CL_Vec4 &color);
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

#endif //GLIMPLEMENTATION_MATERIALUNIFORMCOLORGL_H