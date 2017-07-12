#include <image360/MonoSphere.h>

namespace cl {
MonoSphere::MonoSphere(ILoggerFactory *loggerFactory,
                       IModelFactory *modelFactory,
                       IDiffuseTextureFactory *diffuseTextureFactory,
                       ITransformTreeFactory *transformTreeFactory) {
  assert(loggerFactory != nullptr && modelFactory != nullptr);
  this->logger = loggerFactory->createLogger("StereoSphere:");
  this->modelFactory = modelFactory;
  this->diffuseTextureFactory = diffuseTextureFactory;
  this->transformTreeFactory = transformTreeFactory;
}

void MonoSphere::initialize(Scene *scene, std::unique_ptr<Image> textureImage) {
  // create an empty model to hold the sphere
  std::unique_ptr<Model> imageContainer =
      modelFactory->create("imageContainer");
  assert(imageContainer != nullptr);
  this->monoSphereImageContainer = imageContainer.get();
  scene->addToScene(std::move(imageContainer));

  // create empty texture, diffuse material and diffuse shader for the sphere
  std::unique_ptr<ShaderDiffuseTexture> shaderDiffuseTexture;
  std::unique_ptr<MaterialDiffuseTexture> materialDiffuseTexture;
  std::unique_ptr<Texture> imageTexture;

  // initialize shader and add to scene ~ should I name this more appropriately?
  shaderDiffuseTexture = diffuseTextureFactory->createShader("shader", scene);
  assert(shaderDiffuseTexture != nullptr);
  this->monoSphereShader = shaderDiffuseTexture.get();
  scene->addToScene(std::move(shaderDiffuseTexture));

  // initialize material and add to scene ~ should I name this more
  // appropriately?
  materialDiffuseTexture = diffuseTextureFactory->createMaterial(
      "material", (ShaderDiffuseTexture *)this->monoSphereShader);
  assert(materialDiffuseTexture != nullptr);
  this->monoSphereMaterial = materialDiffuseTexture.get();
  scene->addToScene(std::move(materialDiffuseTexture));

  // initialize texture for the sphere from the input image & add to scene
  imageTexture = diffuseTextureFactory->createTexture("imageTexture");

  std::unique_ptr<Image> image = std::move(textureImage);
  imageTexture->setTextureData(std::move(image->data));
  imageTexture->setHeight(image->height);
  imageTexture->setWidth(image->width);
  imageTexture->setTextureDataSize(image->dataSize);
  this->monoSphereImageTexture = imageTexture.get();
  scene->addToScene(std::move(imageTexture));

  // attach texture to material
  ((MaterialDiffuseTexture *)this->monoSphereMaterial)
      ->setDiffuseTexture(this->monoSphereImageTexture);

  // create and attach a transform tree to our model
  std::unique_ptr<TransformTreeModel> transformSphereUptr =
      transformTreeFactory->createTransformTreeModel(
          this->monoSphereImageContainer);
  this->monoSphereImageContainer->getComponentList().addComponent(
      std::move(transformSphereUptr));

  // set the size and position of the sphere
  TransformTreeModel *transformSphere =
      (TransformTreeModel *)this->monoSphereImageContainer->getComponentList()
          .getComponent("transformTree");
  transformSphere->setLocalPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
  transformSphere->setLocalScale(CL_Vec3(100.0f, 100.0f, 100.0f));

  // link the model with its material
  this->monoSphereImageContainer->createBiRelation(this->monoSphereMaterial);

  // finally ~ populate the model with a sphere
  std::unique_ptr<ModelModifier> modelModifier(new ModelModifier);
  UVSphereBuilder uvSphereBuilder(modelModifier.get());
  uvSphereBuilder.buildUnitSphere(this->monoSphereImageContainer, 5);
}
}