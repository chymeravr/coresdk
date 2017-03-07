#include <glImplementation/renderObjects/opengles3/MaterialUniformColorGLES3.h>

namespace cl{
	MaterialUniformColorGLES3::MaterialUniformColorGLES3(const std::string &sceneId, ShaderUniformColorGLES3 *shader, ILoggerFactory *loggerFactory, CL_Vec4 &color)
		: MaterialUniformColor(sceneId, shader, loggerFactory, color){
		logger = loggerFactory->createLogger("glImplementation::MaterialUniformColorGLES3: ");
	}
	IRenderable *MaterialUniformColorGLES3::getRenderable(){
		return this;
	}
	void MaterialUniformColorGLES3::setColorUniformId(CL_GLuint colorUniformId){
		this->colorUniformId = colorUniformId;
	}
	bool MaterialUniformColorGLES3::initialize(){
		logger->log(LOG_INFO, "material:" + sceneId + " initialized");
		return true;
	}
	void MaterialUniformColorGLES3::draw(){
		glUniform4fv(colorUniformId, 1, &color[0]);
	}
	void MaterialUniformColorGLES3::deinitialize(){
	}
}