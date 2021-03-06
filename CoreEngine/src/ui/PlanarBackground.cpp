#include <coreEngine/ui/PlanarBackground.h>

namespace cl {
PlanarBackground::PlanarBackground(std::string id, IModelFactory *modelFactory,
                                   IUniformColorFactory *uniformColorFactory,
                                   Scene *scene, CL_Vec4 color,
                                   ITransformTreeFactory *transformTreeFactory,
                                   CL_Vec3 &localPosition,
                                   CL_Vec3 &localRotation, float width,
                                   float height,
                                   ILoggerFactory *loggerFactory) {
  std::string modelId;
  

  ShaderUniformColor *shader = uniformColorFactory->getShader(scene);
  this->material = uniformColorFactory->getMaterial(shader, color);

  modelId = id;

  // Creating and adding model to scene
  std::unique_ptr<Model> modelUptr = modelFactory->create(modelId);
  assert(modelUptr != nullptr);
  model = modelUptr.get();
  assert(!scene->exists(modelId));
  scene->addToScene(std::move(modelUptr));

  // the planar backgrounds must have blending enabled to show the background
  // we set the alpha value during construction ~ set to 1.0 to make opaque
  model->setBlending(true);
  model->setDepthTest(true);

  // Creating a rectangle model for ui
  // TODO : use rectangle model builder to do this work in the future
  std::vector<CL_Vec3> &vertices = model->getVertices();
  vertices.push_back(CL_Vec3(-width / 2, -height / 2, 0.0));  // left bottom
  vertices.push_back(CL_Vec3(width / 2, -height / 2, 0.0));   // right bottom
  vertices.push_back(CL_Vec3(width / 2, height / 2, 0.0));    // right top
  vertices.push_back(CL_Vec3(-width / 2, height / 2, 0.0));   // left top
  std::vector<CL_GLuint> &indices = model->getIndices();
  indices.push_back(0);
  indices.push_back(1);
  indices.push_back(3);
  indices.push_back(1);
  indices.push_back(2);
  indices.push_back(3);

  // assigning material
  material->createBiRelation(model);

  std::unique_ptr<TransformTreeModel> transformTreeModel =
      transformTreeFactory->createTransformTreeModel(model);
  transformTreeModel->setLocalPosition(localPosition);
  transformTreeModel->setLocalRotation(localRotation);
  this->setTransformTree(transformTreeModel.get());
  ComponentList &componentList = model->getComponentList();
  componentList.addComponent(std::move(transformTreeModel));
}

// write tests
void PlanarBackground::setColor(CL_Vec4 color) {
  this->material->setColor(color);
}

// write tests
void PlanarBackground::setScale(CL_Vec3 scale){
	TransformTreeModel *transformModel = (TransformTreeModel *)this->model->getComponentList().getComponent("transformTree");
	transformModel->setLocalScale(scale);
}
}