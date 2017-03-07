#include <coreEngine/conf/MathType.h>

namespace cl{
	glm::mat4x4 CL_perspective(float &fov, float &aspect, float &nearPlane, float &farPlane) {
		return glm::perspective(fov, aspect, nearPlane, farPlane);
	}

	CL_Mat44 CL_tweakedInfinitePerspective(float &fovy, float &aspect, float &nearPlane){
		return glm::tweakedInfinitePerspective(fovy, aspect, nearPlane);
	}

	glm::mat4x4 CL_lookAt(CL_Vec3 &cameraPosition, CL_Vec3 &lookAtCenter, CL_Vec3 &up) {
		return glm::lookAt(cameraPosition, lookAtCenter, glm::normalize(up));
	}

	CL_Mat44 CL_RotationMatrix(CL_Quat quaternion){
		return glm::mat4_cast(quaternion);
	}

	CL_Mat44 CL_RotationMatrixX(float angle) {
		return glm::eulerAngleX(angle);
	}

	CL_Mat44 CL_RotationMatrixY(float angle) {
		return glm::eulerAngleY(angle);
	}

	CL_Mat44 CL_RotationMatrixZ(float angle) {
		return glm::eulerAngleZ(angle);
	}

	CL_Mat44 CL_TranlationMatrix(CL_Vec3 position) {
		return glm::translate(CL_IdentityMatrix4X4(), position);
	}

	CL_Mat44 CL_ScaleMatrix(CL_Vec3 scale) {
		return glm::scale(CL_IdentityMatrix4X4(), scale);
	}

	CL_Mat44 CL_IdentityMatrix4X4() {
		return CL_Mat44(1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0);
	}

	CL_Vec3 CL_CrossProduct(CL_Vec3 v1, CL_Vec3 v2){
		return glm::cross(v1, v2);
	}

	CL_Vec3 CL_Normalize(CL_Vec3 v){
		return glm::normalize(v);
	}

	CL_Mat44 CL_Make_Mat44(float mat[16])
	{
		return glm::make_mat4(mat);
	}

	CL_Mat44 CL_Invert_Mat44(CL_Mat44 mat){
		return glm::inverse(mat);
	}

	CL_Vec3 CL_Euler_Angles(CL_Quat quat){
		return glm::eulerAngles(quat);
	}

	CL_Quat CL_Angle_Axis(CL_Vec3 eulerAngles){
		CL_Quat qx = glm::angleAxis(eulerAngles[0], CL_Vec3(1.0f, 0.0f, 0.0f));
		CL_Quat qy = glm::angleAxis(eulerAngles[1], CL_Vec3(0.0f, 1.0f, 0.0f));
		CL_Quat qz = glm::angleAxis(eulerAngles[2], CL_Vec3(0.0f, 0.0f, 1.0f));

		return glm::cross(glm::cross(qy, qx), qz);
	}
}