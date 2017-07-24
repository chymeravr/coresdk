#include <coreEngine/components/transformTree/TransformTreeModel.h>
#include <image360/StereoSphere.h>

namespace cl {
StereoSphere::StereoSphere(ILoggerFactory &loggerFactory,
                           IModelFactory &modelFactory,
                           IDiffuseTextureFactory &diffuseTextureFactory,
                           ITransformTreeFactory &transformTreeFactory,
                           std::unique_ptr<Image> textureImage) {
	/*assert(*loggerFactory != nullptr);
	assert(*modelFactory != nullptr);*/
  this->logger = loggerFactory.createLogger("StereoSphere:");
  this->modelFactory = &modelFactory;
  this->diffuseTextureFactory = &diffuseTextureFactory;
  this->transformTreeFactory = &transformTreeFactory;
  this->textureImage = std::move(textureImage);
}

void StereoSphere::initialize(Scene &scene) {
  // create an empty model for the left sphere and add to the scene
  std::unique_ptr<Model> imageContainerLeft =
      modelFactory->create("imageContainerLeft");
  assert(imageContainerLeft != nullptr);
  this->stereoImageContainer[EYE::LEFT] = imageContainerLeft.get();
  scene.addToScene(std::move(imageContainerLeft));

  // create an empty model for the right sphere and add to the scene
  std::unique_ptr<Model> imageContainerRight =
      modelFactory->create("imageContainerRight");
  assert(imageContainerRight != nullptr);
  this->stereoImageContainer[EYE::RIGHT] = imageContainerRight.get();
  scene.addToScene(std::move(imageContainerRight));

  // declare texture, material and shader for the models
  std::unique_ptr<ShaderDiffuseTexture> shaderDiffuseTexture;
  std::unique_ptr<MaterialDiffuseTexture> materialDiffuseTexture;
  std::unique_ptr<Texture> imageTexture;

  // create a diffuse shader and add to the scene
  shaderDiffuseTexture =
      this->diffuseTextureFactory->createShader("shader", &scene);
  assert(shaderDiffuseTexture != nullptr);
  this->stereoShader = shaderDiffuseTexture.get();
  scene.addToScene(std::move(shaderDiffuseTexture));

  // create a corresponding material and add to the scene
  materialDiffuseTexture = this->diffuseTextureFactory->createMaterial(
      "material", (ShaderDiffuseTexture *)this->stereoShader);
  assert(materialDiffuseTexture != nullptr);
  this->stereoMaterial = materialDiffuseTexture.get();
  scene.addToScene(std::move(materialDiffuseTexture));

  // todo - change texture mapping of models to do this??? REDUNDANT COMMENT NOW
  // ~ REMOVE
  imageTexture = this->diffuseTextureFactory->createTexture("imageTexture");

  // use the image data to create a texture and add to the scene
  // std::unique_ptr<Image> image = std::move(this->textureImage);
  imageTexture->setTextureData(std::move(this->textureImage->data));
  imageTexture->setHeight(this->textureImage->height);
  imageTexture->setWidth(this->textureImage->width);
  imageTexture->setTextureDataSize(this->textureImage->dataSize);
  this->stereoImageTexture = imageTexture.get();
  scene.addToScene(std::move(imageTexture));
  ((MaterialDiffuseTexture *)this->stereoMaterial)
      ->setDiffuseTexture(this->stereoImageTexture);

  // create a transform for the left sphere and attach it to the corresponding
  // model
  std::unique_ptr<TransformTreeModel> leftTransformSphereUptr =
      this->transformTreeFactory->createTransformTreeModel(
          this->stereoImageContainer[EYE::LEFT]);
  this->stereoImageContainer[EYE::LEFT]->getComponentList().addComponent(
      std::move(leftTransformSphereUptr));

  // create a transform for the right sphere and attach it to the corresponding
  // model
  std::unique_ptr<TransformTreeModel> rightTransformSphereUptr =
      this->transformTreeFactory->createTransformTreeModel(
          this->stereoImageContainer[EYE::RIGHT]);
  this->stereoImageContainer[EYE::RIGHT]->getComponentList().addComponent(
      std::move(rightTransformSphereUptr));

  // Set the transform of the left sphere to reflect correct size and position
  TransformTreeModel *transformSphereLeft =
      (TransformTreeModel *)this->stereoImageContainer[EYE::LEFT]
          ->getComponentList()
          .getComponent("transformTree");
  transformSphereLeft->setLocalPosition(LEFT_SPHERE_POSITION);
  transformSphereLeft->setLocalScale(LEFT_SPHERE_SCALE);

  // link the left model with its material
  this->stereoImageContainer[EYE::LEFT]->createBiRelation(this->stereoMaterial);

  // Set the transform of the right sphere to reflect correct size and position
  TransformTreeModel *transformSphereRight =
      (TransformTreeModel *)this->stereoImageContainer[EYE::RIGHT]
          ->getComponentList()
          .getComponent("transformTree");
  transformSphereRight->setLocalPosition(RIGHT_SPHERE_POSITION);
  transformSphereRight->setLocalScale(RIGHT_SPHERE_SCALE);

  // link the right model to material - note we use same material but with diff
  // UV map
  this->stereoImageContainer[EYE::RIGHT]->createBiRelation(
      this->stereoMaterial);

  // Finally, we need to populate the left model with a sphere
  std::unique_ptr<ModelModifier> modelModifierLeft(new ModelModifier);
  UVSphereBuilder uvSphereBuilderLeft(modelModifierLeft.get());
  // bottom half of the image corresponds to the left sphere
  uvSphereBuilderLeft.setVMax(0.5f);
  uvSphereBuilderLeft.buildUnitSphere(this->stereoImageContainer[EYE::LEFT], 5);

  // ... and the right model with a sphere too
  std::unique_ptr<ModelModifier> modelModifierRight(new ModelModifier);
  UVSphereBuilder uvSphereBuilderRight(modelModifierRight.get());
  // top half of the sphere corresponds to the right sphere
  uvSphereBuilderRight.setVMin(0.5f);
  uvSphereBuilderRight.buildUnitSphere(this->stereoImageContainer[EYE::RIGHT],
                                       5);
  // some rendering properties of this model
  this->stereoImageContainer[EYE::LEFT]->setDepthTest(true);
  this->stereoImageContainer[EYE::RIGHT]->setDepthTest(true);
  this->stereoImageContainer[EYE::LEFT]->setBlending(true);
  this->stereoImageContainer[EYE::RIGHT]->setBlending(true);
}

void StereoSphere::preDrawLeft() {
  this->stereoImageContainer[EYE::LEFT]->setIsVisible(true);
  this->stereoImageContainer[EYE::RIGHT]->setIsVisible(false);
}

void StereoSphere::preDrawRight() {
  this->stereoImageContainer[EYE::LEFT]->setIsVisible(false);
  this->stereoImageContainer[EYE::RIGHT]->setIsVisible(true);
}
}