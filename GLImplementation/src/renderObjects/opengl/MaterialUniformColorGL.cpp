#include <glImplementation/renderObjects/opengl/MaterialUniformColorGL.h>

namespace cl{
	MaterialUniformColorGL::MaterialUniformColorGL(const std::string &sceneId, ShaderUniformColorGL *shader, ILoggerFactory *loggerFactory, CL_Vec4 &color)
		: MaterialUniformColor(sceneId, shader, loggerFactory, color){
		logger = loggerFactory->createLogger("glImplementation::MaterialUniformColorGL: ");
	}
	IRenderable *MaterialUniformColorGL::getRenderable(){
		return this;
	}
	void MaterialUniformColorGL::setColorUniformId(CL_GLuint colorUniformId){
		this->colorUniformId = colorUniformId;
	}
	bool MaterialUniformColorGL::initialize(){
		logger->log(LOG_INFO, "material:" + sceneId + " initialized");
		return true;
	}
	void MaterialUniformColorGL::draw(){
		glUniform4fv(colorUniformId, 1, &color[0]);
	}
	void MaterialUniformColorGL::deinitialize(){
	}
}