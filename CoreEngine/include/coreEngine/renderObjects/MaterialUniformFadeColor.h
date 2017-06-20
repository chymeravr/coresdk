#ifndef COREENGINE_MATERIALUNIFORMFADECOLOR_H
#define COREENGINE_MATERIALUNIFORMFADECOLOR_H

#include <coreEngine/renderObjects/Material.h>
#include <coreEngine/renderObjects/ShaderUniformFadeColor.h>
#include <coreEngine/renderObjects/Texture.h>

namespace cl {
class MaterialUniformFadeColor : public Material {
 public:
  virtual ~MaterialUniformFadeColor() {}
  MaterialUniformFadeColor(const std::string &sceneId,
                           ShaderUniformFadeColor *shader,
                           ILoggerFactory *loggerFactory, CL_Vec4 &color)
      : Material(sceneId, shader, loggerFactory) {
    this->color = color;
  }
  void setColor(CL_Vec4 &color) { this->color = color; }
  CL_Vec4 &getColor() { return this->color; }

 protected:
  CL_Vec4 color;
};
}

#endif  // COREENGINE_MATERIALUNIFORMFADECOLOR_H