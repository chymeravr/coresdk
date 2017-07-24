#include <coreEngine/modelBuilder/ModelLoader.h>
#include <coreEngine/modelBuilder/RectangleBuilder.h>
#include <image360/Controller.h>

namespace cl {
Controller::Controller(ILoggerFactory &loggerFactory,
                       IModelFactory &modelFactory,
                       ITransformTreeFactory &transformTreeFactory,
                       IDiffuseTextureFactory &diffuseTextureFactory,
                       UIFactory &uiFactory,
                       std::unique_ptr<Image> controllerImage,
                       std::unique_ptr<Image> laserBeamImage,
                       std::string controllerModelPath) {
  assert(controllerImage != nullptr);
  assert(laserBeamImage != nullptr);
  assert(!controllerModelPath.empty());

  this->loggerFactory = &loggerFactory;
  this->modelFactory = &modelFactory;
  this->transformTreeFactory = &transformTreeFactory;
  this->diffuseTextureFactory = &diffuseTextureFactory;
  this->uiFactory = &uiFactory;

  this->controllerImage = std::move(controllerImage);
  this->laserBeamImage = std::move(laserBeamImage);
  this->controllerModelPath = controllerModelPath;
}

void Controller::initialize(Scene &scene) {
  this->initializeController(scene);
  this->initializeControllerLaser(scene);
  this->initializeControllerReticle(scene);
}

void Controller::initializeController(Scene &scene) {
  std::unique_ptr<Model> controllerModelContainer =
      modelFactory->create("controllerModel");
  assert(controllerModelContainer != nullptr);
  this->controllerModel = controllerModelContainer.get();
  scene.addToScene(std::move(controllerModelContainer));
  std::unique_ptr<ModelLoader> modelLoader =
      std::unique_ptr<ModelLoader>(new ModelLoader(this->loggerFactory));
  modelLoader->load_obj(this->controllerModelPath, this->controllerModel);
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

  transformController->setLocalPosition(CONTROLLER_POSITION);
  transformController->setLocalScale(CONTROLLER_SCALE);
  transformController->setLocalRotation(CONTROLLER_ROTATION);

  std::unique_ptr<ShaderDiffuseTexture> controllerShaderUptr;
  std::unique_ptr<MaterialDiffuseTexture> controllerMaterialUptr;
  std::unique_ptr<Texture> controllerTextureUptr;

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

  std::unique_ptr<Image> controllerImageUptr = std::move(this->controllerImage);
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

void Controller::initializeControllerLaser(Scene &scene) {
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
  transformLaserBeam->setLocalPosition(LASER_BEAM_POSITION);
  transformLaserBeam->setLocalScale(LASER_BEAM_SCALE);
  transformLaserBeam->setLocalRotation(LASER_BEAM_ROTATION);

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

  std::unique_ptr<Image> laserBeamImageUptr = std::move(this->laserBeamImage);
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
  TransformTreeModel *transformController =
      (TransformTreeModel *)this->controllerModel->getComponentList()
          .getComponent("transformTree");
  this->controllerReticle = uiFactory->createReticle(
      "controllerReticle", &scene, transformController, RETICLE_COLOR);
  TransformTreeModel *transformReticle =
      this->controllerReticle->getTransformTreeModel();
  transformReticle->setLocalPosition(RETICLE_POSITION);
  transformReticle->setLocalScale(RETICLE_SCALE);
}

TransformTreeModel *Controller::getGazeTransformSource() {
  return (TransformTreeModel *)this->controllerModel->getComponentList()
      .getComponent("transformTree");
}

void Controller::updateControllerQuaternion(CL_Quat controllerOrientation) {
  TransformTreeModel *transformController =
      (TransformTreeModel *)this->controllerModel->getComponentList()
          .getComponent("transformTree");
  transformController->setLocalQuaternion(controllerOrientation);
}
void Controller::updateControllerRotation(CL_Vec3 controllerRotation) {
  TransformTreeModel *transformController =
      (TransformTreeModel *)this->controllerModel->getComponentList()
          .getComponent("transformTree");
  transformController->setLocalRotation(controllerRotation);
}
void Controller::updateControllerPosition(CL_Vec3 controllerPosition) {
  TransformTreeModel *transformController =
      (TransformTreeModel *)this->controllerModel->getComponentList()
          .getComponent("transformTree");
  transformController->setLocalPosition(controllerPosition);
}
}