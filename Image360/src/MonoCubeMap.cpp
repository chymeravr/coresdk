#include <coreEngine/modifier/ImageModifier.h>
#include <image360/MonoCubeMap.h>

namespace cl {
MonoCubeMap::MonoCubeMap(
    ILoggerFactory *loggerFactory, IModelFactory *modelFactory,
    IDiffuseTextureCubeMapFactory *diffuseTextureCubeMapFactory,
    ITransformTreeFactory *transformTreeFactory) {
  assert(loggerFactory != nullptr && modelFactory != nullptr);
  this->logger = loggerFactory->createLogger("StereoSphere:");
  this->modelFactory = modelFactory;
  this->diffuseTextureCubeMapFactory = diffuseTextureCubeMapFactory;
  this->transformTreeFactory = transformTreeFactory;
}
void MonoCubeMap::initialize(Scene *scene,
                             std::unique_ptr<Image> textureImage) {
  // create an empty model to hold the cube
  std::unique_ptr<Model> imageContainer =
      modelFactory->create("imageContainer");
  assert(imageContainer != nullptr);
  this->monoCubeMapImageContainer = imageContainer.get();
  std::unique_ptr<ModelModifier> modelModifier(new ModelModifier);
  CubeBuilder cubeBuilder(modelModifier.get());
  cubeBuilder.buildInwardCube(this->monoCubeMapImageContainer);
  scene->addToScene(std::move(imageContainer));

  // set the transform parameters for the model
  std::unique_ptr<TransformTreeModel> transformSphereUptr =
      this->transformTreeFactory->createTransformTreeModel(
          this->monoCubeMapImageContainer);
  this->monoCubeMapImageContainer->getComponentList().addComponent(
      std::move(transformSphereUptr));

  TransformTreeModel *transformSphere =
      (TransformTreeModel *)this->monoCubeMapImageContainer->getComponentList()
          .getComponent("transformTree");
  transformSphere->setLocalPosition(CL_Vec3(0.0f, 0.0f, 0.0f));
  transformSphere->setLocalScale(CL_Vec3(100.0f, 100.0f, 100.0f));

  // initialize the cube map shader
  std::unique_ptr<ShaderDiffuseTextureCubeMap> shaderUPtr =
      diffuseTextureCubeMapFactory->createShader("shader", scene);
  assert(shaderUPtr != nullptr);
  this->monoCubeMapShader = shaderUPtr.get();
  scene->addToScene(std::move(shaderUPtr));

  // intialize cubemap material
  std::unique_ptr<MaterialDiffuseTextureCubeMap> materialUPtr =
      diffuseTextureCubeMapFactory->createMaterial(
          "material", (ShaderDiffuseTextureCubeMap *)this->monoCubeMapShader);
  assert(materialUPtr != nullptr);
  this->monoCubeMapMaterial = materialUPtr.get();
  scene->addToScene(std::move(materialUPtr));

  // 6 images for each texture on cube faces
  std::unique_ptr<Image> frontImage(new Image);
  std::unique_ptr<Image> leftImage(new Image);
  std::unique_ptr<Image> backImage(new Image);
  std::unique_ptr<Image> rightImage(new Image);
  std::unique_ptr<Image> topImage(new Image);
  std::unique_ptr<Image> bottomImage(new Image);

  // populate each image by extracting the relavent face from the cubemap image
  assert(textureImage != nullptr);
  std::unique_ptr<Image> originalImage = std::move(textureImage);
  ImageModifier imageModifier(logger.get());
  imageModifier.convertSingleCubicImageIntoSix(
      originalImage.get(), frontImage.get(), leftImage.get(), backImage.get(),
      rightImage.get(), bottomImage.get(), topImage.get());

  assert(rightImage != nullptr && leftImage != nullptr && topImage != nullptr &&
         bottomImage != nullptr && backImage != nullptr &&
         frontImage != nullptr);

  initCubeMapTexture(scene, rightImage.get(), leftImage.get(), topImage.get(),
                     bottomImage.get(), frontImage.get(), backImage.get());
}

void MonoCubeMap::initCubeMapTexture(Scene *scene, Image *rightImage,
                                     Image *leftImage, Image *topImage,
                                     Image *bottomImage, Image *frontImage,
                                     Image *backImage) {
  std::unique_ptr<TextureCubeMap> textureUPtr =
      diffuseTextureCubeMapFactory->createTexture("imageTexture");
  assert(textureUPtr != nullptr);

  // using an array of images initialized in same order as the texture enum to
  // enable looping
  Image *cubeMapImages[6] = {leftImage, rightImage, frontImage,
                             backImage, topImage,   bottomImage};
  for (int i = 0; i < 6; i++) {
    TEXTURE_CUBE_MAP_FACE cubeMapTexture =
        static_cast<TEXTURE_CUBE_MAP_FACE>(i);

    textureUPtr->setTextureData(cubeMapTexture,
                                std::move(cubeMapImages[i]->data));
    textureUPtr->setHeight(cubeMapTexture, cubeMapImages[i]->height);
    textureUPtr->setWidth(cubeMapTexture, cubeMapImages[i]->width);
    textureUPtr->setTextureDataSize(cubeMapTexture, cubeMapImages[i]->dataSize);
  }

  this->monoCubeMapImageTexture = textureUPtr.get();
  scene->addToScene(std::move(textureUPtr));

  // set texture to material
  ((MaterialDiffuseTextureCubeMap *)this->monoCubeMapMaterial)
      ->setDiffuseTexture(this->monoCubeMapImageTexture);

  // create link between model and material
  this->monoCubeMapImageContainer->createBiRelation(this->monoCubeMapMaterial);
}
}