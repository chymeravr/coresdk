#ifndef IMAGE360_CONTROLLER_H
#define IMAGE360_CONTROLLER_H

#include <coreEngine/components/transformTree/ITransformTreeFactory.h>
#include <coreEngine/factory/IDiffuseTextureFactory.h>
#include <coreEngine/factory/IModelFactory.h>
#include <coreEngine/modifier/Image.h>
#include <coreEngine/ui/UIFactory.h>

#include <image360/ApplicationObject.h>

namespace cl {
class Controller : public ApplicationObject {
 public:
  Controller(ILoggerFactory &loggerFactory, IModelFactory &modelFactory,
             ITransformTreeFactory &transformTreeFactory,
             IDiffuseTextureFactory &diffuseTextureFactory,
             UIFactory &uiFactory, std::unique_ptr<Image> controllerImage,
             std::unique_ptr<Image> laserBeamImage,
             std::string controllerModelPath);

  void initialize(Scene &scene);

  Model *getControllerModel() { return this->controllerModel; }
  Reticle *getControllerReticle() { return this->controllerReticle.get(); }
  TransformTreeModel *getGazeTransformSource();

  void updateControllerQuaternion(CL_Quat controllerOrientation);
  void updateControllerRotation(CL_Vec3 controllerRotation);
  void updateControllerPosition(CL_Vec3 controllerPosition);

 private:
  // private functions
  void initializeController(Scene &scene);
  void initializeControllerLaser(Scene &scene);
  void initializeControllerReticle(Scene &scene);

  // Controller Component
  Shader *controllerShader;
  Material *controllerMaterial;
  Texture *controllerTexture;
  Model *controllerModel;
  std::unique_ptr<Image> controllerImage;
  std::string controllerModelPath;

  // Laser Beam Component
  Shader *laserBeamShader;
  Material *laserBeamMaterial;
  Texture *laserBeamTexture;
  Model *laserBeamModel;
  std::unique_ptr<Image> laserBeamImage;

  // Reticle at the end of controller's laser beam
  std::unique_ptr<Reticle> controllerReticle;

  ILoggerFactory *loggerFactory;
  std::unique_ptr<ILogger> logger;

  // factories
  IModelFactory *modelFactory;
  IDiffuseTextureFactory *diffuseTextureFactory;
  ITransformTreeFactory *transformTreeFactory;
  UIFactory *uiFactory;

  // CONSTANTS
  CL_Vec3 CONTROLLER_POSITION = CL_Vec3(0.65f, -0.65f, -1.0f);
  CL_Vec3 CONTROLLER_SCALE = CL_Vec3(25.0f, 25.0f, 25.0f);
  CL_Vec3 CONTROLLER_ROTATION = CL_Vec3(30.0f, 0.0f, 30.0f);

  CL_Vec3 LASER_BEAM_POSITION = CL_Vec3(0.0f, 0.0f, -0.125f);
  CL_Vec3 LASER_BEAM_SCALE = CL_Vec3(0.025f, 0.1f, 0.1f);
  CL_Vec3 LASER_BEAM_ROTATION = CL_Vec3(270.0f, 0.0f, 0.0f);

  CL_Vec3 RETICLE_POSITION = CL_Vec3(0.0f, 0.0f, -0.35f);
  CL_Vec3 RETICLE_SCALE = CL_Vec3(0.03f, 0.03f, 0.02f);
  CL_Vec4 RETICLE_COLOR = CL_Vec4(1.0f, 1.0f, 1.0f, 1.0f);
};
}

#endif  // IMAGE360_CONTROLLER_H