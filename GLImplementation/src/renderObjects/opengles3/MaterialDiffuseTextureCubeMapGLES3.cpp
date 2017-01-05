#include <glImplementation/renderObjects/opengles3/MaterialDiffuseTextureCubeMapGLES3.h>
#include <glImplementation/renderObjects/TextureCubeMapGL.h>

namespace cl
{
	MaterialDiffuseTextureCubeMapGLES3::MaterialDiffuseTextureCubeMapGLES3(const std::string &sceneId, 
																		   ShaderDiffuseTextureCubeMapGLES3 *shader, 
																		   ILoggerFactory *loggerFactory)
		: MaterialDiffuseTextureCubeMap(sceneId, shader, loggerFactory)
	{
		logger = loggerFactory->createLogger("glImplementation::MaterialDiffuseTextureGLES3: ");
	}
	IRenderable *MaterialDiffuseTextureCubeMapGLES3::getRenderable()
	{
		return this;
	}
	void MaterialDiffuseTextureCubeMapGLES3::setDiffuseTextureId(CL_GLuint diffuseTextureUniformId)
	{
		this->diffuseTextureUniformId = diffuseTextureUniformId;
	}
	bool MaterialDiffuseTextureCubeMapGLES3::initialize()
	{
		IRenderable *textureRenderable = this->diffuseTexture->getRenderable();
		assert(textureRenderable != nullptr);
		textureRenderable->initialize();
		logger->log(LOG_INFO, "material:" + sceneId + " initialized");
		return true;
	}
	void MaterialDiffuseTextureCubeMapGLES3::draw()
	{
		// Bind our diffuse texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		this->diffuseTexture->getRenderable()->draw();
		glUniform1i(diffuseTextureUniformId, 0);
	}
	void MaterialDiffuseTextureCubeMapGLES3::deinitialize()
	{
		this->diffuseTexture->getRenderable()->deinitialize();
	}
}