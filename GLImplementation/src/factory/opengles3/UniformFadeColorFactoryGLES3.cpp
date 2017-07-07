#include <glImplementation/factory/UniformFadeColorFactoryGL.h>

#include <sstream>

namespace cl {
// android version of std does not have a to_string
template <typename T>
std::string to_string(T value) {
  std::ostringstream os;
  os << value;
  return os.str();
}

UniformFadeColorFactoryGL::UniformFadeColorFactoryGL(
    ILoggerFactory *loggerFactory) {
  assert(loggerFactory != nullptr);
  this->loggerFactory = loggerFactory;
}
ShaderUniformFadeColor *UniformFadeColorFactoryGL::getShader(Scene *scene) {
  std::string shaderId = "ShaderUniformFadeColorGLES3";
  ShaderUniformFadeColor *shader = nullptr;
  if (!(shader = (ShaderUniformFadeColor *)scene->getFromScene(shaderId))) {
    // creating shader
    std::unique_ptr<ShaderUniformFadeColor> shaderUptr(
        new ShaderUniformFadeColorGL(shaderId, loggerFactory, scene));
    assert(shaderUptr != nullptr);
    shader = shaderUptr.get();
    scene->addToScene(std::move(shaderUptr));
  }
  return shader;
}
MaterialUniformFadeColor *UniformFadeColorFactoryGL::getMaterial(
    ShaderUniformFadeColor *shader, CL_Vec4 &color) {
  assert(shader != nullptr);
  std::vector<Relation *> sceneRelations = shader->getRelations("scene");
  assert(sceneRelations.size() == 1);
  Scene *scene = (Scene *)sceneRelations[0];
  std::string materialId = "MaterialUniformFadeColor" +
                           cl::to_string(color[0]) + cl::to_string(color[1]) +
                           cl::to_string(color[2]) + cl::to_string(color[3]);
  MaterialUniformFadeColor *material = nullptr;
  if (!(material =
            (MaterialUniformFadeColor *)scene->getFromScene(materialId))) {
    // creating material
    std::unique_ptr<MaterialUniformFadeColor> materialUptr(
        new MaterialUniformFadeColorGL(materialId,
                                       (ShaderUniformFadeColorGL *)shader,
                                       loggerFactory, color));
    assert(materialUptr != nullptr);
    material = materialUptr.get();
    scene->addToScene(std::move(materialUptr));
  }
  return material;
}
}