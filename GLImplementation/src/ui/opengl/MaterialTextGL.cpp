#include <glImplementation/ui/opengl/MaterialTextGL.h>

namespace cl{
	MaterialTextGL::MaterialTextGL(const std::string &sceneId, ShaderTextGL *shader, TextureTextGL *texture, ILoggerFactory *loggerFactory)
		: MaterialText(sceneId, shader, texture, loggerFactory){
		logger = loggerFactory->createLogger("glImplementation::MaterialTextGL: ");
	}
	IRenderable *MaterialTextGL::getRenderable(){
		return this;
	}
	void MaterialTextGL::setTextTextureId(CL_GLuint textTextureUniformId){
		this->textTextureUniformId = textTextureUniformId;
	}
	void MaterialTextGL::setColorId(CL_GLuint colorUniformId){
		this->colorUniformId = colorUniformId;
	}
	bool MaterialTextGL::initialize(){
		IRenderable *textureRenderable = this->textureText->getRenderable();
		assert(textureRenderable != nullptr);
		textureRenderable->initialize();
		logger->log(LOG_INFO, "material:" + sceneId + " initialized");
		return true;
	}
	void MaterialTextGL::draw(){
		glUniform4fv(colorUniformId, 1, &color[0]);
		glActiveTexture(GL_TEXTURE0);
		this->textureText->getRenderable()->draw();
		glUniform1i(textTextureUniformId, 0);
	}
	void MaterialTextGL::deinitialize(){
		this->textureText->getRenderable()->deinitialize();
	}
}