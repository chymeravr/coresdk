#include <coreEngine/components/gazeDetector/GazeDetectorBox.h>

namespace cl {
GazeDetectorBox::GazeDetectorBox(
    std::string &idOfObjectComponentAttachedTo, TransformTree *shooterTransform,
    TransformTree *gazeObjectTransform, EventGazeListener *gazeListener,
    GazeDetectorContainer *gazeDetectorContainer, CL_Vec3 &shooterLocalOrigin,
    CL_Vec3 &shooterLocalLookAt, float lengthX, float lengthY, float lengthZ)
    : GazeDetector(idOfObjectComponentAttachedTo, shooterTransform,
                   gazeObjectTransform, gazeListener, gazeDetectorContainer) {
  this->lengthX = lengthX;
  this->lengthY = lengthY;
  this->lengthZ = lengthZ;
  this->shooterLocalOrigin = shooterLocalOrigin;
  this->shooterLocalLookAt = shooterLocalLookAt;
}
bool GazeDetectorBox::getIsGazed() {
  /*
  Line: r = L0 + (L1-L0)t
  Plane: 0 = N.(r-P0)
  N = (P4-P0)X(P1-P0) --> P0=left-top-vertex P1=left-bottom-vertex
  P3=right-top-vertex
  t = N.(P0-L0)/N.(L1-L0)
  */

  // Establishing line in local coordinates
  CL_Vec4 L0(shooterLocalOrigin[0], shooterLocalOrigin[1],
             shooterLocalOrigin[2], 1.0f);
  CL_Vec4 L1(shooterLocalLookAt[0], shooterLocalLookAt[1],
             shooterLocalLookAt[2], 1.0f);

  // Line in global coordinates
  CL_Mat44 lineGlobalMatrix = shooterTransform->getGlobalTransform();
  L0 = lineGlobalMatrix * L0;
  L1 = lineGlobalMatrix * L1;
  CL_Vec3 L0Vec3 = CL_Vec3(L0[0], L0[1], L0[2]);
  CL_Vec3 L1Vec3 = CL_Vec3(L1[0], L1[1], L1[2]);

  CL_Vec4 P0;
  CL_Vec4 P1;
  CL_Vec4 P3;
  CL_Vec3 P0Vec3;
  CL_Vec3 P1Vec3;
  CL_Vec3 P3Vec3;

  CL_Mat44 planeGlobalMatrix = gazeObjectTransform->getGlobalTransform();
  // Front Face -->local coordinates
  P0 =
      planeGlobalMatrix * CL_Vec4(-lengthX / 2, lengthY / 2, lengthZ / 2, 1.0f);
  P1 = planeGlobalMatrix *
       CL_Vec4(-lengthX / 2, -lengthY / 2, lengthZ / 2, 1.0f);
  P3 = planeGlobalMatrix * CL_Vec4(lengthX / 2, lengthY / 2, lengthZ / 2, 1.0f);
  P0Vec3 = CL_Vec3(P0[0], P0[1], P0[2]);
  P1Vec3 = CL_Vec3(P1[0], P1[1], P1[2]);
  P3Vec3 = CL_Vec3(P3[0], P3[1], P3[2]);
  if (intersectionWithPlane(P0Vec3, P1Vec3, P3Vec3, L0Vec3, L1Vec3))
    return true;

  // Back Face -->local coordinates
  P0 = planeGlobalMatrix *
       CL_Vec4(-lengthX / 2, lengthY / 2, -lengthZ / 2, 1.0f);
  P1 = planeGlobalMatrix *
       CL_Vec4(-lengthX / 2, -lengthY / 2, -lengthZ / 2, 1.0f);
  P3 =
      planeGlobalMatrix * CL_Vec4(lengthX / 2, lengthY / 2, -lengthZ / 2, 1.0f);
  P0Vec3 = CL_Vec3(P0[0], P0[1], P0[2]);
  P1Vec3 = CL_Vec3(P1[0], P1[1], P1[2]);
  P3Vec3 = CL_Vec3(P3[0], P3[1], P3[2]);
  if (intersectionWithPlane(P0Vec3, P1Vec3, P3Vec3, L0Vec3, L1Vec3))
    return true;

  // Left Face -->local coordinates
  P0 = planeGlobalMatrix *
       CL_Vec4(-lengthX / 2, lengthY / 2, -lengthZ / 2, 1.0f);
  P1 = planeGlobalMatrix *
       CL_Vec4(-lengthX / 2, -lengthY / 2, -lengthZ / 2, 1.0f);
  P3 =
      planeGlobalMatrix * CL_Vec4(-lengthX / 2, lengthY / 2, lengthZ / 2, 1.0f);
  P0Vec3 = CL_Vec3(P0[0], P0[1], P0[2]);
  P1Vec3 = CL_Vec3(P1[0], P1[1], P1[2]);
  P3Vec3 = CL_Vec3(P3[0], P3[1], P3[2]);
  if (intersectionWithPlane(P0Vec3, P1Vec3, P3Vec3, L0Vec3, L1Vec3))
    return true;

  // Right Face -->local coordinates
  P0 =
      planeGlobalMatrix * CL_Vec4(lengthX / 2, lengthY / 2, -lengthZ / 2, 1.0f);
  P1 = planeGlobalMatrix *
       CL_Vec4(lengthX / 2, -lengthY / 2, -lengthZ / 2, 1.0f);
  P3 = planeGlobalMatrix * CL_Vec4(lengthX / 2, lengthY / 2, lengthZ / 2, 1.0f);
  P0Vec3 = CL_Vec3(P0[0], P0[1], P0[2]);
  P1Vec3 = CL_Vec3(P1[0], P1[1], P1[2]);
  P3Vec3 = CL_Vec3(P3[0], P3[1], P3[2]);
  if (intersectionWithPlane(P0Vec3, P1Vec3, P3Vec3, L0Vec3, L1Vec3))
    return true;

  // Top Face -->local coordinates
  P0 = planeGlobalMatrix *
       CL_Vec4(-lengthX / 2, lengthY / 2, -lengthZ / 2, 1.0f);
  P1 =
      planeGlobalMatrix * CL_Vec4(-lengthX / 2, lengthY / 2, lengthZ / 2, 1.0f);
  P3 =
      planeGlobalMatrix * CL_Vec4(lengthX / 2, lengthY / 2, -lengthZ / 2, 1.0f);
  P0Vec3 = CL_Vec3(P0[0], P0[1], P0[2]);
  P1Vec3 = CL_Vec3(P1[0], P1[1], P1[2]);
  P3Vec3 = CL_Vec3(P3[0], P3[1], P3[2]);
  if (intersectionWithPlane(P0Vec3, P1Vec3, P3Vec3, L0Vec3, L1Vec3))
    return true;

  // Bottom Face -->local coordinates
  P0 = planeGlobalMatrix *
       CL_Vec4(-lengthX / 2, -lengthY / 2, -lengthZ / 2, 1.0f);
  P1 = planeGlobalMatrix *
       CL_Vec4(-lengthX / 2, -lengthY / 2, lengthZ / 2, 1.0f);
  P3 = planeGlobalMatrix *
       CL_Vec4(lengthX / 2, -lengthY / 2, -lengthZ / 2, 1.0f);
  P0Vec3 = CL_Vec3(P0[0], P0[1], P0[2]);
  P1Vec3 = CL_Vec3(P1[0], P1[1], P1[2]);
  P3Vec3 = CL_Vec3(P3[0], P3[1], P3[2]);
  if (intersectionWithPlane(P0Vec3, P1Vec3, P3Vec3, L0Vec3, L1Vec3))
    return true;

  return false;
}
float GazeDetectorBox::getDistanceFromShooter() {
  CL_Vec4 shooterOrigin4f =
      shooterTransform->getGlobalTransform() * CL_Vec4(0.0f, 0.0f, 0.0f, 1.0f);
  CL_Vec4 gazeObjectOrigin4f = gazeObjectTransform->getGlobalTransform() *
                               CL_Vec4(0.0f, 0.0f, 0.0f, 1.0f);
  CL_Vec3 shooterOrigin3f(shooterOrigin4f[0], shooterOrigin4f[1],
                          shooterOrigin4f[2]);
  CL_Vec3 gazeObjectOrigin3f(gazeObjectOrigin4f[0], gazeObjectOrigin4f[1],
                             gazeObjectOrigin4f[2]);
  auto dotPInput = gazeObjectOrigin3f - shooterOrigin3f;
  return dotProduct(dotPInput, dotPInput);
}
bool GazeDetectorBox::intersectionWithPlane(CL_Vec3 &P0, CL_Vec3 &P1,
                                            CL_Vec3 &P3, CL_Vec3 &L0,
                                            CL_Vec3 &L1) {
  CL_Vec3 N = CL_CrossProduct((P3 - P0), (P1 - P0));

  auto l1l0 = L1 - L0;
  float denominator = dotProduct(N, l1l0);
  if (denominator == 0) {
    return false;
  }
  auto p0l0 = P0 - L0;
  float t = dotProduct(N, p0l0) / denominator;
  if (t < 0) return false;
  CL_Vec3 r = L0 + (L1 - L0) * t;  // line plane intersection point

  // Calculating if a point r is inside rectangle formed by P0, P1, P2, P3
  // satisfy following condition
  // (0 < (r0-P0).(P1-P0) < (P1-P0).(P1-P0) ) and (0 < (r0-P0).(P3-P0) <
  // (P3-P0).(P3-P0))

  auto rp0 = r - P0;
  auto p1p0 = P1 - P0;
  float rp01dot = dotProduct(rp0, p1p0);
  float p01dot = dotProduct(p1p0, p1p0);
  if (rp01dot <= 0) return false;
  if (p01dot <= rp01dot) return false;

  auto p3p0 = P3 - P0;
  float rp03dot = dotProduct(rp0, p3p0);
  float p03dot = dotProduct(p3p0, p3p0);
  if (rp03dot <= 0) return false;
  if (p03dot <= rp03dot) return false;
  return true;
}
float GazeDetectorBox::dotProduct(CL_Vec3 &v1, CL_Vec3 &v2) {
  return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}
}