#include <glImplementation/renderObjects/MaterialDiffuseTextureGL.h>
#include <glImplementation/renderObjects/TextureGL.h>

namespace cl{
    MaterialDiffuseTextureGL::MaterialDiffuseTextureGL(const std::string &sceneId, ShaderDiffuseTextureGL *shader, ILoggerFactory *loggerFactory)
    : MaterialDiffuseTexture(sceneId, shader, loggerFactory){
        logger = loggerFactory->createLogger("glImplementation::MaterialDiffuseTextureGL: ");
    }
    IRenderable *MaterialDiffuseTextureGL::getRenderable(){
        return this;
    }
    void MaterialDiffuseTextureGL::setDiffuseTextureId(CL_GLuint diffuseTextureUniformId){
        this->diffuseTextureUniformId = diffuseTextureUniformId;
    }
    bool MaterialDiffuseTextureGL::initialize(){
        IRenderable *textureRenderable = this->diffuseTexture->getRenderable();
        assert(textureRenderable != nullptr);
        textureRenderable->initialize();
        logger->log(LOG_INFO, "material:" + sceneId + " initialized");
        return true;
    }
    void MaterialDiffuseTextureGL::draw(){
        // Bind our diffuse texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        this->diffuseTexture->getRenderable()->draw();
        glUniform1i(diffuseTextureUniformId, 0);
    }
    void MaterialDiffuseTextureGL::deinitialize(){
        this->diffuseTexture->getRenderable()->deinitialize();
    }
}