#include <glImplementation/factory/UniformFadeColorFactoryGL.h>

// to_string is not implemented in android - hence different implementations
namespace cl {

UniformFadeColorFactoryGL::UniformFadeColorFactoryGL(
    ILoggerFactory *loggerFactory) {
  assert(loggerFactory != nullptr);
  this->loggerFactory = loggerFactory;
}
ShaderUniformFadeColor *UniformFadeColorFactoryGL::getShader(Scene *scene) {
  std::string shaderId = "ShaderUniformFadeColorGL";
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
                           std::to_string(color[0]) + std::to_string(color[1]) +
                           std::to_string(color[2]) + std::to_string(color[3]);
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