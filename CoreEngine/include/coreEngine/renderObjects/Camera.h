#ifndef COREENGINE_CAMERA_H
#define COREENGINE_CAMERA_H

#include <coreEngine/conf/Types.h>
#include <coreEngine/renderObjects/ComponentList.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <coreEngine/renderObjects/IScenable.h>
#include <coreEngine/renderObjects/Relation.h>
#include <coreEngine/renderObjects/Scene.h>

namespace cl {
class Camera : public IScenable, public Relation {
 public:
  virtual ~Camera() {}
  Camera(const std::string &sceneId, ILoggerFactory *loggerFactory);
         //Scene *scene);
  virtual IRenderable *getRenderable() = 0;
  std::string getSceneId();
  std::string getType();
  std::string getUniqueIdentifier();
  ComponentList &getComponentList() { return componentList; }
  virtual void calculateViewMatrix();
  virtual void calculateProjectionMatrix();
  CL_Mat44 getViewMatrix();
  CL_Mat44 getProjectionMatrix();
  void setLocation(const CL_Vec3 &location);
  void setLookAtPoint(const CL_Vec3 &lookAtPoint);
  void setUpVector(const CL_Vec3 &upVector);
  /**
  * fov is in radians. It represents vertical field of view from top to down.
  */
  void setFov(const float &fov);

  /**
  * aspect ratio = width/height
  */
  void setAspect(const float &aspect);
  void setNearPlane(const float &nearPlane);
  void setFarPlane(const float &farPlane);

  void setIsAsymetricProjection(bool isAsymetric);
  void setLeft(const float &left);
  void setRight(const float &right);
  void setTop(const float &top);
  void setBottom(const float &bottom);

  CL_Vec3 getLocalLookAtPoint();
  CL_Vec3 getLocalUpVector();
  CL_Vec3 getLocalLocation();

 protected:
  std::string sceneId = "";
  const std::string type = "camera";
  CL_Vec3 location;
  CL_Vec3 lookAtPoint;
  CL_Vec3 upVector;
  CL_Mat44 viewMatrix;
  CL_Mat44 projectionMatrix;

  // assymetric projection is used when left and right viewing frustums have
  // different parameters for calculating projection matrix
  bool isAsymetricProjection = false;

  // explicitly get frustum paramters if projection frustum is assymetric
  float left;
  float right;
  float top;
  float bottom;
  // use fov and aspect if projection frustum is symmetric
  /**
  * fov is in radians. It represents vertical field of view from top to down.
  */
  float fov;
  /**
  * aspect ratio = width/height
  */
  float aspect;

  float nearPlane;
  float farPlane;
  CL_Vec3 localLocation = CL_Vec3(0.0, 0.0, 0.0);
  CL_Vec3 localLookAtPoint = CL_Vec3(0.0, 0.0, -1.0);
  CL_Vec3 localUpVector = CL_Vec3(0.0, 1.0, 0.0);
  ComponentList componentList;
};
}

#endif  // COREENGINE_CAMERA_H