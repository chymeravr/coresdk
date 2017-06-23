#include <coreEngine/ui/CharacterElement.h>

namespace cl {
CharacterElement::CharacterElement(
    std::string id, CL_Vec2 origin, CL_GLfloat height, CL_GLfloat width,
    MaterialText *material, IModelFactory *modelFactory, Scene *scene,
    ITransformTreeFactory *transformTreeFactory) {
  assert(material != nullptr);
  assert(modelFactory != nullptr);
  assert(scene != nullptr);
  assert(transformTreeFactory != nullptr);

  this->id = id;
  this->scene = scene;
  this->material = material;

  assert(!scene->exists(id));
  std::unique_ptr<Model> modelUptr = modelFactory->create(id);
  std::vector<CL_Vec3> &vertices = modelUptr->getVertices();
  std::vector<CL_Vec2> &uvs = modelUptr->getUvs();
  std::vector<CL_GLuint> &indices = modelUptr->getIndices();
  Model *model = modelUptr.get();

  // our characters must have blending enabled to render correctly.
  // We also need depth testing to render the reticle correctly
  // when it hovers over our characters.
  model->setDepthTest(true);
  model->setBlending(true);

  scene->addToScene(std::move(modelUptr));
  vertices.push_back(CL_Vec3(origin[0], origin[1] + height, 0.0));
  uvs.push_back(CL_Vec2(0.0, 0.0));
  vertices.push_back(CL_Vec3(origin[0], origin[1], 0.0));
  uvs.push_back(CL_Vec2(0.0, 1.0));
  vertices.push_back(CL_Vec3(origin[0] + width, origin[1], 0.0));
  uvs.push_back(CL_Vec2(1.0, 1.0));
  vertices.push_back(CL_Vec3(origin[0] + width, origin[1] + height, 0.0));
  uvs.push_back(CL_Vec2(1.0, 0.0));

  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(2);
  indices.push_back(0);
  indices.push_back(2);
  indices.push_back(3);

  material->createBiRelation(model);

  std::unique_ptr<TransformTreeModel> transformTreeModel =
      transformTreeFactory->createTransformTreeModel(model);
  this->setTransformTree(transformTreeModel.get());
  ComponentList &componentList = model->getComponentList();
  componentList.addComponent(std::move(transformTreeModel));
}

void CharacterElement::deleteElement() {
  Model *model = (Model *)scene->getFromScene(id);
  model->destroyBiRelation(material);
  scene->removeFromScene(model);
}
}