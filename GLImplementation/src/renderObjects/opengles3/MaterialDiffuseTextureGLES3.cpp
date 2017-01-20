#include <glImplementation/renderObjects/opengles3/MaterialDiffuseTextureGLES3.h>
#include <glImplementation/renderObjects/TextureGL.h>

namespace cl{
	MaterialDiffuseTextureGLES3::MaterialDiffuseTextureGLES3(const std::string &sceneId, ShaderDiffuseTextureGLES3 *shader, ILoggerFactory *loggerFactory)
		: MaterialDiffuseTexture(sceneId, shader, loggerFactory){
		logger = loggerFactory->createLogger("glImplementation::MaterialDiffuseTextureGLES3: ");
	}
	IRenderable *MaterialDiffuseTextureGLES3::getRenderable(){
		return this;
	}
	void MaterialDiffuseTextureGLES3::setDiffuseTextureId(CL_GLuint diffuseTextureUniformId){
		this->diffuseTextureUniformId = diffuseTextureUniformId;
	}
	bool MaterialDiffuseTextureGLES3::initialize(){
		IRenderable *textureRenderable = this->diffuseTexture->getRenderable();
		assert(textureRenderable != nullptr);
		textureRenderable->initialize();
		logger->log(LOG_INFO, "material:" + sceneId + " initialized");
		return true;
	}
	void MaterialDiffuseTextureGLES3::draw(){
		// Bind our diffuse texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		this->diffuseTexture->getRenderable()->draw();
		glUniform1i(diffuseTextureUniformId, 0);
	}
	void MaterialDiffuseTextureGLES3::deinitialize(){
		this->diffuseTexture->getRenderable()->deinitialize();
	}
}