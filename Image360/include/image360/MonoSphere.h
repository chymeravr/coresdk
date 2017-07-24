#ifndef IMAGE360_MONOSPHERE_H
#define IMAGE360_MONOSPHERE_H

/*
* This class is responsible to creating a spheres with 360 equirectangular image
* texture mapped to it. This class works with mono images.
* For rendering Stereo Images use StereoSphere instead
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

// factories
#include <coreEngine/components/transformTree/ITransformTreeFactory.h>
#include <coreEngine/factory/IDiffuseTextureFactory.h>
#include <coreEngine/factory/IModelFactory.h>

// utils
#include <coreEngine/util/ILogger.h>
#include <coreEngine/util/ILoggerFactory.h>

#include <image360/ApplicationObject.h>
#include <image360/Constants.h>

namespace cl {
class MonoSphere : public ApplicationObject {
 public:
  MonoSphere(ILoggerFactory &loggerFactory, IModelFactory &modelFactory,
             IDiffuseTextureFactory &diffuseTextureFactory,
             ITransformTreeFactory &transformTreeFactory,
             std::unique_ptr<Image> textureImage);
  void initialize(Scene &scene);

 private:
  // rendering components
  Shader *monoSphereShader;
  Material *monoSphereMaterial;
  Texture *monoSphereImageTexture;
  Model *monoSphereImageContainer;
  std::unique_ptr<Image> textureImage;
  // logging util
  std::unique_ptr<ILogger> logger;

  // factories
  IModelFactory *modelFactory;
  IDiffuseTextureFactory *diffuseTextureFactory;
  ITransformTreeFactory *transformTreeFactory;

  // CONSTANTS
  CL_Vec3 SPHERE_POSITION = CL_Vec3(0.0f, 0.0f, 0.0f);
  CL_Vec3 SPHERE_SCALE = CL_Vec3(100.0f, 100.0f, 100.0f);

  int N_SUBDIVISION_TIMES = 5;
};
}

#endif  // IMAGE360_MONOSPHERE_H