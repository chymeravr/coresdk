#include <coreEngine/modelBuilder/ModelLoader.h>
#include <coreEngine/modelBuilder/RectangleBuilder.h>
#include <image360/Controller.h>

namespace cl {
Controller::Controller(ILoggerFactory &loggerFactory,
                       IModelFactory &modelFactory,
                       ITransformTreeFactory &transformTreeFactory,
                       IDiffuseTextureFactory &diffuseTextureFactory,
                       UIFactory &uiFactory) {
  this->loggerFactory = &loggerFactory;
  this->modelFactory = &modelFactory;
  this->transformTreeFactory = &transformTreeFactory;
  this->diffuseTextureFactory = &diffuseTextureFactory;
  this->uiFactory = &uiFactory;
}
void Controller::initializeController(Scene &scene,
                                      std::unique_ptr<Image> controllerImage,
                                      std::string controllerModelPath) {
  // todo :
  // 1. create object model
  // 2. initialize diffuse shader
  // 3. map shader to model texture
  std::unique_ptr<Model> controllerModelContainer =
      modelFactory->create("controllerModel");
  assert(controllerModelContainer != nullptr);
  this->controllerModel = controllerModelContainer.get();
  scene.addToScene(std::move(controllerModelContainer));
  std::unique_ptr<ModelLoader> modelLoader =
      std::unique_ptr<ModelLoader>(new ModelLoader(this->loggerFactory));
  modelLoader->load_obj(controllerModelPath, this->controllerModel);
  this->controllerModel->setDepthTest(true);
  this->controllerModel->setBlending(true);
  this->controllerModel->setBackFaceCulling(true);

  std::unique_ptr<TransformTreeModel> controllerTransformUptr =
      this->transformTreeFactory->createTransformTreeModel(
          this->controllerModel);
  this->controllerModel->getComponentList().addComponent(
      std::move(controllerTransformUptr));

  TransformTreeModel *transformController =
      (TransformTreeModel *)this->controllerModel->getComponentList()
          .getComponent("transformTree");

  transformController->setLocalPosition(CL_Vec3(0.65f, -0.65f, -1.0f));
  transformController->setLocalScale(CL_Vec3(25.0f, 25.0f, 25.0f));
  transformController->setLocalRotation(CL_Vec3(30.0f, 0.0f, 30.0f));

  std::unique_ptr<ShaderDiffuseTexture> controllerShaderUptr;
  std::unique_ptr<MaterialDiffuseTexture> controllerMaterialUptr;
  std::unique_ptr<Texture> controllerTextureUptr;

  // doubt ~ why is this named shader for 360 degree spheres
  controllerShaderUptr =
      this->diffuseTextureFactory->createShader("controllerShader", &scene);
  assert(controllerShaderUptr != nullptr);
  this->controllerShader = controllerShaderUptr.get();
  controllerShaderUptr->enableAlphaChannel();
  scene.addToScene(std::move(controllerShaderUptr));

  controllerMaterialUptr = this->diffuseTextureFactory->createMaterial(
      "controllerMaterial", (ShaderDiffuseTexture *)this->controllerShader);
  assert(controllerMaterialUptr != nullptr);
  this->controllerMaterial = controllerMaterialUptr.get();
  scene.addToScene(std::move(controllerMaterialUptr));

  controllerTextureUptr =
      this->diffuseTextureFactory->createTexture("controllerTexture");

  std::unique_ptr<Image> controllerImageUptr = std::move(controllerImage);
  controllerTextureUptr->setTextureData(std::move(controllerImageUptr->data));
  controllerTextureUptr->setHeight(controllerImageUptr->height);
  controllerTextureUptr->setWidth(controllerImageUptr->width);
  controllerTextureUptr->setTextureDataSize(controllerImageUptr->dataSize);
  controllerTextureUptr->setColorFormat(Texture::ColorFormat::RGBA);
  //   controllerTextureUptr->setColorFormat(Texture::ColorFormat::RGB);

  this->controllerTexture = controllerTextureUptr.get();
  scene.addToScene(std::move(controllerTextureUptr));
  ((MaterialDiffuseTexture *)this->controllerMaterial)
      ->setDiffuseTexture(this->controllerTexture);

  this->controllerModel->createBiRelation(this->controllerMaterial);
  return;
}

void Controller::initializeControllerLaser(
    Scene &scene, std::unique_ptr<Image> laserBeamImage) {
  std::unique_ptr<Model> laserBeamModelUptr = modelFactory->create("laserBeam");
  assert(laserBeamModelUptr != nullptr);
  this->laserBeamModel = laserBeamModelUptr.get();
  scene.addToScene(std::move(laserBeamModelUptr));
  std::unique_ptr<RectangleBuilder> rectangleBuilder =
      std::unique_ptr<RectangleBuilder>(
          new RectangleBuilder(this->loggerFactory));
  rectangleBuilder->buildRectangle(this->laserBeamModel, 0.01f, 2.5f);
  this->laserBeamModel->setDepthTest(true);
  this->laserBeamModel->setBlending(true);

  std::unique_ptr<TransformTreeModel> laserBeamTransformUptr =
      this->transformTreeFactory->createTransformTreeModel(
          this->laserBeamModel);
  this->laserBeamModel->getComponentList().addComponent(
      std::move(laserBeamTransformUptr));

  TransformTreeModel *transformController =
      (TransformTreeModel *)this->controllerModel->getComponentList()
          .getComponent("transformTree");

  TransformTreeModel *transformLaserBeam =
      (TransformTreeModel *)this->laserBeamModel->getComponentList()
          .getComponent("transformTree");

  transformController->addChild(transformLaserBeam);
  transformLaserBeam->setLocalPosition(CL_Vec3(0.0f, 0.0f, -0.125f));
  transformLaserBeam->setLocalScale(CL_Vec3(0.025, 0.1f, 0.1f));
  transformLaserBeam->setLocalRotation(CL_Vec3(270.0f, 0.0f, 0.0f));

  std::unique_ptr<ShaderDiffuseTexture> laserBeamShaderUptr;
  std::unique_ptr<MaterialDiffuseTexture> laserBeamMaterialUptr;
  std::unique_ptr<Texture> laserBeamTextureUptr;

  // doubt ~ why is this named shader for 360 degree spheres
  laserBeamShaderUptr =
      this->diffuseTextureFactory->createShader("laserBeamShader", &scene);
  assert(laserBeamShaderUptr != nullptr);
  this->laserBeamShader = laserBeamShaderUptr.get();
  laserBeamShaderUptr->enableAlphaChannel();
  scene.addToScene(std::move(laserBeamShaderUptr));

  laserBeamMaterialUptr = this->diffuseTextureFactory->createMaterial(
      "laserBeamMaterial", (ShaderDiffuseTexture *)this->laserBeamShader);
  assert(laserBeamMaterialUptr != nullptr);
  this->laserBeamMaterial = laserBeamMaterialUptr.get();
  scene.addToScene(std::move(laserBeamMaterialUptr));

  laserBeamTextureUptr =
      this->diffuseTextureFactory->createTexture("laserBeamTexture");

  std::unique_ptr<Image> laserBeamImageUptr = std::move(laserBeamImage);
  laserBeamTextureUptr->setTextureData(std::move(laserBeamImageUptr->data));
  laserBeamTextureUptr->setHeight(laserBeamImageUptr->height);
  laserBeamTextureUptr->setWidth(laserBeamImageUptr->width);
  laserBeamTextureUptr->setTextureDataSize(laserBeamImageUptr->dataSize);
  laserBeamTextureUptr->setColorFormat(Texture::ColorFormat::RGBA);

  this->laserBeamTexture = laserBeamTextureUptr.get();
  scene.addToScene(std::move(laserBeamTextureUptr));
  ((MaterialDiffuseTexture *)this->laserBeamMaterial)
      ->setDiffuseTexture(this->laserBeamTexture);

  this->laserBeamModel->createBiRelation(this->laserBeamMaterial);
}

void Controller::initializeControllerReticle(Scene &scene) {
  // create a reticle attached to the controller
  auto reticleColor = CL_Vec4(1.0f, 1.0f, 1.0f, 1.0f);
  TransformTreeModel *transformController =
      (TransformTreeModel *)this->controllerModel->getComponentList()
          .getComponent("transformTree");
  this->controllerReticle = uiFactory->createReticle(
      "controllerReticle", &scene, transformController, reticleColor);
  TransformTreeModel *transformReticle =
      this->controllerReticle->getTransformTreeModel();
  transformReticle->setLocalPosition(CL_Vec3(0.0f, 0.0f, -0.35f));
  transformReticle->setLocalScale(CL_Vec3(0.03f, 0.03f, 0.02f));
}

TransformTree *Controller::getGazeTransformTarget() {
  return (TransformTreeModel *)this->controllerModel->getComponentList()
      .getComponent("transformTree");
}
}