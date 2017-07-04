#ifndef COREENGINE_SHADERDIFFUSETEXTURE_H
#define COREENGINE_SHADERDIFFUSETEXTURE_H

#include <coreEngine/renderObjects/Shader.h>

namespace cl {
class ShaderDiffuseTexture : public Shader {
 public:
  virtual ~ShaderDiffuseTexture() {}
  ShaderDiffuseTexture(const std::string &sceneId,
                       ILoggerFactory *loggerFactory, Scene *scene)
      : Shader(sceneId, loggerFactory, scene) {}

  void enableAlphaChannel() { this->isAlphaPresent = true; }
  bool isAlphaChannelEnabled() { return this->isAlphaPresent; }

 private:
  bool isAlphaPresent = false;
};
}

#endif  // COREENGINE_SHADERDIFFUSETEXTURE_H