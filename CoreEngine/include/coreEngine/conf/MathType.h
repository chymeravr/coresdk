//
// Created by chimeralabs on 10/13/2016.
//

#ifndef ANDROIDSDK_MATHTYPE_H
#define ANDROIDSDK_MATHTYPE_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace cl
{
#define CL_PI 3.14159265358979323846

typedef glm::vec2 CL_Vec2;
typedef glm::vec3 CL_Vec3;
typedef glm::vec4 CL_Vec4;
typedef glm::mat4x4 CL_Mat44;
typedef glm::quat CL_Quat;
/**
	 * Wrapper function for getting symmteric perspective camera matrix
	 *
	 * @param fov: Field of View
	 * @param aspect: Aspect ratio of camera (horizontal/vertical)
	 * @param nearPlane: Distance from camera nearer to which scene will be culled
	 * @param farPlane: Dstance from camera farther from which scene will be culled
	 *
	 * @return A 4x4 matrix that represents the perspective matrix
	 */
CL_Mat44 CL_perspective(float &fov, float &aspect, float &nearPlane, float &farPlane);
CL_Mat44 CL_tweakedInfinitePerspective(float &fovy, float &aspect, float &nearPlane);

/**
	 * Wrapper function for getting asymmteric perspective camera matrix
	 *
	 * @param left: left x coordinate of projection plane
	 * @param right: right x coordinate of projection plane
	 * @param top: top y coordinate of projection plane
	 * @param bottom: bottom y coordinate of projection plane
	 * @param nearPlane: Distance from camera nearer to which scene will be culled
	 * @param farPlane: Dstance from camera farther from which scene will be culled
	 *
	 * @return A 4x4 matrix that represents the perspective matrix
	 */
CL_Mat44 CL_frustum(float &left, float &right, float &bottom, float &top, float &nearPlane, float &farPlane);
//CL_Mat44 CL_infinteFrustum(float &left, float &right, float &bottom, float &top, float &nearPlane);

/**
	 * Wrapper function for getting lookat camera view matrix
	 *
	 * @param cameraPosition: The location where center of camera is located
	 * @param lookAtCenter: Position where the center of camera is looking at
	 * @param up: Up vector of the camera
	 *
	 * @return: A 4x4 matrix that represent the view matrix
	 */

CL_Mat44 CL_lookAt(CL_Vec3 &cameraPosition, CL_Vec3 &lookAtCenter, CL_Vec3 &up);

/**
	 * Get Rotation Matrix from Quaternion
	 */
CL_Mat44 CL_RotationMatrix(CL_Quat quaternion);
/**
	 * Get Rotation Matrix around X axis
	 *
	 * @param angle: angle of rotation. Angles in radians.
	 *
	 * @return A 4x4 matrix representing the rotation
	 */
CL_Mat44 CL_RotationMatrixX(float angle);

/**
	 * Get Rotation Matrix around Y axis.
	 *
	 * @param angle: angle of rotation. Angles in radians.
	 *
	 * @return A 4x4 matrix representing the rotation
	 */
CL_Mat44 CL_RotationMatrixY(float angle);

/**
	 * Get Rotation Matrix around Y axis
	 *
	 * @param angle: angle of rotation. Angles in radians.
	 *
	 * @return A 4x4 matrix representing the rotation
	 */
CL_Mat44 CL_RotationMatrixZ(float angle);

/**
	 * Get Translation Matrix
	 *
	 * @param position: position to which translation should happen
	 *
	 */
CL_Mat44 CL_TranlationMatrix(CL_Vec3 position);

/**
	 * Get Scale Matrix
	 *
	 * @param scale vector
	 */
CL_Mat44 CL_ScaleMatrix(CL_Vec3 scale);

CL_Mat44 CL_IdentityMatrix4X4();

CL_Vec3 CL_CrossProduct(CL_Vec3 v1, CL_Vec3 v2);

CL_Vec3 CL_Normalize(CL_Vec3 v);

CL_Mat44 CL_Make_Mat44(float mat[16]);

CL_Vec3 CL_Euler_Angles(CL_Quat quat);

CL_Mat44 CL_Invert_Mat44(CL_Mat44 mat);

CL_Quat CL_Angle_Axis(CL_Vec3 axis);

CL_Quat CL_Rot_To_Quat(CL_Mat44 mat);

CL_Mat44 CL_MultMat(CL_Mat44 mat, CL_Mat44 mat2);
}

#endif //ANDROIDSDK_MATHTYPE_H
