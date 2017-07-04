#ifndef GLIMPLEMENTATION_SHADERDIFFUSETEXTUREGL_H
#define GLIMPLEMENTATION_SHADERDIFFUSETEXTUREGL_H

#include <coreEngine/renderObjects/IRenderable.h>
#include <coreEngine/renderObjects/ShaderDiffuseTexture.h>
#include <glImplementation/renderObjects/CameraGLContainer.h>
#include <glImplementation/renderObjects/Shaders.h>

namespace cl {
class ShaderDiffuseTextureGL : public ShaderDiffuseTexture, public IRenderable {
 public:
  ShaderDiffuseTextureGL(const std::string &sceneId,
                         ILoggerFactory *loggerFactory, Scene *scene);
  IRenderable *getRenderable();
  /*
          * Here in initialize, set the program id to all those who need it
   * (Model, Material etc.)
          */
  bool initialize();
  void draw();
  void deinitialize();

 private:
  std::unique_ptr<ILogger> logger;
  CL_GLuint programId;
  std::unique_ptr<CameraGLContainer> cameraGLContainer;

  // glsl vertex and fragment shaders
  std::string vertexShaderSrc = diffuseTextureGlVertexShader;
  std::string fragmentShaderSrc = diffuseTextureGlFragmentShader;
};
}

#endif  // GLIMPLEMENTATION_SHADERDIFFUSETEXTUREGL_H