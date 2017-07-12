#ifndef IMAGE360_MONOSPHERE_H
#define IMAGE360_MONOSPHERE_H

/*
* This class is responsible to creating two spheres with 360 stereo image
* texture mapped to it. These two image textures correspond to the left
* and right eye and are either Computer Generated or Captured with a
* stereo camera
*/

// renderable objects
#include <coreEngine/renderObjects/Material.h>
#include <coreEngine/renderObjects/Model.h>
#include <coreEngine/renderObjects/Scene.h>
#include <coreEngine/renderObjects/Shader.h>
#include <coreEngine/renderObjects/Texture.h>

// modifier
#include <coreEngine/modifier/Image.h>

// model builders
#include <coreEngine/modelBuilder/UVSphereBuilder.h>

// factory declarations
#include <coreEngine/components/transformTree/ITransformTreeFactory.h>
#include <coreEngine/factory/IDiffuseTextureFactory.h>
#include <coreEngine/factory/IModelFactory.h>

// utils
#include <coreEngine/util/ILogger.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <image360/Constants.h>

namespace cl {
class MonoSphere {
 public:
  MonoSphere(ILoggerFactory *loggerFactory, IModelFactory *modelFactory,
             IDiffuseTextureFactory *diffuseTextureFactory,
             ITransformTreeFactory *transformTreeFactory);
  void initialize(Scene *scene, std::unique_ptr<Image> textureImage);
  void drawLeft();
  void drawRight();

 private:
  // stereo rendering component
  Shader *monoSphereShader;
  Material *monoSphereMaterial;
  Texture *monoSphereImageTexture;
  Model *monoSphereImageContainer;

  // logging util
  std::unique_ptr<ILogger> logger;

  // factories
  IModelFactory *modelFactory;
  IDiffuseTextureFactory *diffuseTextureFactory;
  ITransformTreeFactory *transformTreeFactory;
};
}

#endif  // IMAGE360_MONOSPHERE_H