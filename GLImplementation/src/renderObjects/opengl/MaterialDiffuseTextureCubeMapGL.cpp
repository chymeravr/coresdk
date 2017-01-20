#include <glImplementation/renderObjects/opengl/MaterialDiffuseTextureCubeMapGL.h>
#include <glImplementation/renderObjects/TextureCubeMapGL.h>

namespace cl
{
	MaterialDiffuseTextureCubeMapGL::MaterialDiffuseTextureCubeMapGL(const std::string &sceneId, ShaderDiffuseTextureCubeMapGL *shader, ILoggerFactory *loggerFactory)
		: MaterialDiffuseTextureCubeMap(sceneId, shader, loggerFactory)
	{
		logger = loggerFactory->createLogger("glImplementation::MaterialDiffuseTextureGL: ");
	}
	IRenderable *MaterialDiffuseTextureCubeMapGL::getRenderable()
	{
		return this;
	}
	void MaterialDiffuseTextureCubeMapGL::setDiffuseTextureId(CL_GLuint diffuseTextureUniformId)
	{
		this->diffuseTextureUniformId = diffuseTextureUniformId;
	}
	bool MaterialDiffuseTextureCubeMapGL::initialize()
	{
		IRenderable *textureRenderable = this->diffuseTexture->getRenderable();
		assert(textureRenderable != nullptr);
		textureRenderable->initialize();
		logger->log(LOG_INFO, "material:" + sceneId + " initialized");
		return true;
	}
	void MaterialDiffuseTextureCubeMapGL::draw()
	{
		// Bind our diffuse texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		this->diffuseTexture->getRenderable()->draw();
		glUniform1i(diffuseTextureUniformId, 0);
	}
	void MaterialDiffuseTextureCubeMapGL::deinitialize()
	{
		this->diffuseTexture->getRenderable()->deinitialize();
	}
}