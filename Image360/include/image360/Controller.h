#ifndef IMAGE360_CONTROLLER_H
#define IMAGE360_CONTROLLER_H

#include <coreEngine/components/transformTree/ITransformTreeFactory.h>
#include <coreEngine/factory/IDiffuseTextureFactory.h>
#include <coreEngine/factory/IModelFactory.h>
#include <coreEngine/modifier/Image.h>
#include <coreEngine/ui/UIFactory.h>

namespace cl {
class Controller {
 public:
  Controller(ILoggerFactory &loggerFactory, IModelFactory &modelFactory,
             ITransformTreeFactory &transformTreeFactory,
             IDiffuseTextureFactory &diffuseTextureFactory,
             UIFactory &uiFactory);
  void initializeController(Scene &scene,
                            std::unique_ptr<Image> controllerImage,
                            std::string controllerModelPath);
  void initializeControllerLaser(Scene &scene,
                                 std::unique_ptr<Image> laserBeamImage);
  void initializeControllerReticle(Scene &scene);
  Model *getControllerModel() { return this->controllerModel; }
  Reticle *getControllerReticle() { return this->controllerReticle.get(); }
  TransformTree *getGazeTransformTarget();

 private:
  // Controller Component
  Shader *controllerShader;
  Material *controllerMaterial;
  Texture *controllerTexture;
  Model *controllerModel;

  // Laser Beam Component
  Shader *laserBeamShader;
  Material *laserBeamMaterial;
  Texture *laserBeamTexture;
  Model *laserBeamModel;

  // Reticle at the end of controller's laser beam
  std::unique_ptr<Reticle> controllerReticle;

  ILoggerFactory *loggerFactory;
  std::unique_ptr<ILogger> logger;

  // factories
  IModelFactory *modelFactory;
  IDiffuseTextureFactory *diffuseTextureFactory;
  ITransformTreeFactory *transformTreeFactory;
  UIFactory *uiFactory;
};
}

#endif  // IMAGE360_CONTROLLER_H