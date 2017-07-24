#ifndef IMAGE360_MONOCUBEMAP_H
#define IMAGE360_MONOCUBEMAP_H

/*
* This class is responsible to creating a cube with 360 cubemap image
* texture mapped to it. A cube map format is recommended for 360 images
* by John Cormack. As of now we are only implementing a mono cube map
* viewer. Its trivial to extend the idea to a stereo cube map viewer.
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
#include <coreEngine/modelBuilder/CubeBuilder.h>

// factory declarations
#include <coreEngine/components/transformTree/ITransformTreeFactory.h>
#include <coreEngine/factory/IDiffuseTextureCubeMapFactory.h>
#include <coreEngine/factory/IModelFactory.h>

// utils
#include <coreEngine/util/ILogger.h>
#include <coreEngine/util/ILoggerFactory.h>

#include <image360/ApplicationObject.h>
#include <image360/Constants.h>

namespace cl {
class MonoCubeMap : public ApplicationObject {
 public:
  MonoCubeMap(ILoggerFactory &loggerFactory, IModelFactory &modelFactory,
              IDiffuseTextureCubeMapFactory &diffuseTextureCubeMapFactory,
              ITransformTreeFactory &transformTreeFactory,
              std::unique_ptr<Image> textureImage);
  void initialize(Scene &scene);

 private:
  // mono rendering component
  Shader *monoCubeMapShader;
  Material *monoCubeMapMaterial;
  Texture *monoCubeMapImageTexture;
  Model *monoCubeMapImageContainer;
  std::unique_ptr<Image> textureImage;

  // logging util
  std::unique_ptr<ILogger> logger;

  // factories
  IModelFactory *modelFactory;
  IDiffuseTextureCubeMapFactory *diffuseTextureCubeMapFactory;
  ITransformTreeFactory *transformTreeFactory;

  // CONSTANTS
  CL_Vec3 CUBE_POSITION = CL_Vec3(0.0f, 0.0f, 0.0f);
  CL_Vec3 CUBE_SCALE = CL_Vec3(100.0f, 100.0f, 100.0f);

  void initCubeMapTexture(Scene *scene, Image *rightImage, Image *leftImage,
                          Image *topImage, Image *bottomImage,
                          Image *frontImage, Image *backImage);
};
}

#endif  // IMAGE360_MONOCUBEMAP_H