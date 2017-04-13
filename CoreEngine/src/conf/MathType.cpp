#include <coreEngine/conf/MathType.h>

namespace cl
{
    CL_Mat44 CL_perspective(float &fov, float &aspect, float &nearPlane, float &farPlane)
{
    return glm::perspective(fov, aspect, nearPlane, farPlane);
}
    CL_Mat44 CL_frustum(float &left, float &right, float &bottom, float &top, float &nearPlane, float &farPlane)
    {
        return glm::frustum(left, right, bottom, top, nearPlane, farPlane);
    }

CL_Mat44 CL_tweakedInfinitePerspective(float &fovy, float &aspect, float &nearPlane)
{
    return glm::tweakedInfinitePerspective(fovy, aspect, nearPlane);
}
//    CL_Mat44 CL_infinteFrustum(float &left, float &right, float &top, float &bottom, float &nearPlane){
//
//    }

    CL_Mat44 CL_lookAt(CL_Vec3 &cameraPosition, CL_Vec3 &lookAtCenter, CL_Vec3 &up)
{
    return glm::lookAt(cameraPosition, lookAtCenter, glm::normalize(up));
}

CL_Mat44 CL_RotationMatrix(CL_Quat quaternion)
{
    return glm::mat4_cast(quaternion);
}

CL_Mat44 CL_RotationMatrixX(float angle)
{
    return glm::eulerAngleX(angle);
}

CL_Mat44 CL_RotationMatrixY(float angle)
{
    return glm::eulerAngleY(angle);
}

CL_Mat44 CL_MultMat(CL_Mat44 mat, CL_Mat44 mat2){
    return glm::matrixCompMult(mat, mat2);
}

CL_Mat44 CL_RotationMatrixZ(float angle)
{
    return glm::eulerAngleZ(angle);
}

CL_Mat44 CL_TranlationMatrix(CL_Vec3 position)
{
    return glm::translate(CL_IdentityMatrix4X4(), position);
}

CL_Mat44 CL_ScaleMatrix(CL_Vec3 scale)
{
    return glm::scale(CL_IdentityMatrix4X4(), scale);
}

CL_Mat44 CL_IdentityMatrix4X4()
{
    return CL_Mat44(1.0, 0.0, 0.0, 0.0,
		    0.0, 1.0, 0.0, 0.0,
		    0.0, 0.0, 1.0, 0.0,
		    0.0, 0.0, 0.0, 1.0);
}

CL_Vec3 CL_CrossProduct(CL_Vec3 v1, CL_Vec3 v2)
{
    return glm::cross(v1, v2);
}

CL_Vec3 CL_Normalize(CL_Vec3 v)
{
    return glm::normalize(v);
}

CL_Mat44 CL_Make_Mat44(float mat[16])
{
    return glm::make_mat4(mat);
}

CL_Mat44 CL_Invert_Mat44(CL_Mat44 mat)
{
    return glm::inverse(mat);
}

CL_Vec3 CL_Euler_Angles(CL_Quat quat)
{
    return glm::eulerAngles(quat);
}

CL_Quat CL_Angle_Axis(CL_Vec3 eulerAngles)
{
    //CL_Quat qx = glm::angleAxis(eulerAngles[0], CL_Vec3(1.0f, 0.0f, 0.0f));
    //CL_Quat qy = glm::angleAxis(eulerAngles[1], CL_Vec3(0.0f, 1.0f, 0.0f));
    //CL_Quat qz = glm::angleAxis(eulerAngles[2], CL_Vec3(0.0f, 0.0f, 1.0f));

    //return glm::cross(glm::cross(qy, qx), qz);
    //return (qy * qx) * qz;

    /*auto result = CL_Quat(0.0f, 0.0f, 0.0f, 0.0f);
		auto roll = eulerAngles[0];
		auto pitch = eulerAngles[1];
		auto yaw = eulerAngles[2];
		
		auto t0 = std::cos(yaw * 0.5);
		auto t1 = std::sin(yaw * 0.5);
		auto t2 = std::cos(roll * 0.5);
		auto t3 = std::sin(roll * 0.5);
		auto t4 = std::cos(pitch * 0.5);
		auto t5 = std::sin(pitch * 0.5);

		result.w = t0 * t2 * t4 + t1 * t3 * t5;
		result.x = t0 * t3 * t4 - t1 * t2 * t5;
		result.y = t0 * t2 * t5 + t1 * t3 * t4;
		result.z = t1 * t2 * t4 - t0 * t3 * t5;
		return result;*/

    CL_Mat44 Rx = CL_RotationMatrixX(-eulerAngles[0] * CL_PI / 180);
    CL_Mat44 Ry = CL_RotationMatrixY(-eulerAngles[1] * CL_PI / 180);
    CL_Mat44 Rz = CL_RotationMatrixZ(-eulerAngles[2] * CL_PI / 180);

    auto R = Ry * Rx * Rz;

    float b1 = 0.0f, b2 = 0.0f, b3 = 0.0f, b4 = 0.0f;

    b1 = 0.5 * (std::sqrt(1.0f + R[0][0] + R[1][1] + R[2][2]));
    b2 = (R[2][1] - R[1][2]) / (4 * b1);
    b3 = (R[0][2] - R[2][0]) / (4 * b1);
    b4 = (R[1][0] - R[0][1]) / (4 * b1);

    return CL_Quat(b1, b2, b3, b4);
}
CL_Quat CL_Rot_To_Quat(CL_Mat44 rotmat)
{
    return glm::quat_cast(rotmat);
}
}