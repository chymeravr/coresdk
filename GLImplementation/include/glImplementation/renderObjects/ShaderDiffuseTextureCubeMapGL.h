#ifndef GLIMPLEMENTATION_SHADERDIFFUSETEXTURECUBEMAPGL_H
#define GLIMPLEMENTATION_SHADERDIFFUSETEXTURECUBEMAPGL_H

#include <coreEngine/renderObjects/IRenderable.h>
#include <coreEngine/renderObjects/ShaderDiffuseTextureCubeMap.h>
#include <glImplementation/renderObjects/CameraGLContainer.h>
#include <glImplementation/renderObjects/Shaders.h>

namespace cl {
class ShaderDiffuseTextureCubeMapGL : public ShaderDiffuseTextureCubeMap,
                                      public IRenderable {
 public:
  ShaderDiffuseTextureCubeMapGL(const std::string &sceneId,
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

  std::string vertexShaderSrc = diffuseTextureCubeMapGlVertexShader;

  std::string fragmentShaderSrc = diffuseTextureCubeMapGlFragmentShader;
};
}

#endif  // GLIMPLEMENTATION_SHADERDIFFUSETEXTURECUBEMAPGL_H