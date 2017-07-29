#ifndef IMAGE360_STEREOSPHERE_H
#define IMAGE360_STEREOSPHERE_H

/*
* This class is responsible to creating two spheres with 360 equirectangular
* stereo image texture mapped to it. These two image textures correspond to the
* left and right eye and are either Computer Generated or Captured with a
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

#include <image360/ApplicationObject.h>
#include <image360/StereoObject.h>

namespace cl {
class StereoSphere : public ApplicationObject, public StereoObject {
 public:
  StereoSphere(ILoggerFactory &loggerFactory, IModelFactory &modelFactory,
               IDiffuseTextureFactory &diffuseTextureFactory,
               ITransformTreeFactory &transformTreeFactory,
               std::unique_ptr<Image> textureImage);
  ~StereoSphere();
  void initialize(Scene &scene);
  void preDrawLeft();
  void preDrawRight();

 private:
  // stereo rendering component
  Shader *stereoShader;
  Material *stereoMaterial;
  Texture *stereoImageTexture;
  Model *stereoImageContainer[2];  // 0->left, 1->right
  std::unique_ptr<Image> textureImage;

  // logging util
  std::unique_ptr<ILogger> logger;

  // factories
  IModelFactory *modelFactory;
  IDiffuseTextureFactory *diffuseTextureFactory;
  ITransformTreeFactory *transformTreeFactory;

  // CONSTANTS
  CL_Vec3 LEFT_SPHERE_POSITION = CL_Vec3(0.0f, 0.0f, 0.0f);
  CL_Vec3 LEFT_SPHERE_SCALE = CL_Vec3(100.0f, 100.0f, 100.0f);

  CL_Vec3 RIGHT_SPHERE_POSITION = CL_Vec3(0.0f, 0.0f, 0.0f);
  CL_Vec3 RIGHT_SPHERE_SCALE = CL_Vec3(100.0f, 100.0f, 100.0f);
};
}

#endif  // IMAGE360_STEREOSPHERE_H