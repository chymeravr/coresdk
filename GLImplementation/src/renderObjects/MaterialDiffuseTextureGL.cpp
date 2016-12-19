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
        ((TextureGL*)diffuseTexture)->initialize();
    }
    void MaterialDiffuseTextureGL::draw(){
        // Bind our diffuse texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ((TextureGL*)diffuseTexture)->getTextureId());
        glUniform1i(diffuseTextureUniformId, 0);
    }
    void MaterialDiffuseTextureGL::deinitialize(){

    }
}