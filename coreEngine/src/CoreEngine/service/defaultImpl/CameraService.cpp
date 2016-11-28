//
// Created by chimeralabs on 10/26/2016.
//

#include <coreEngine/service/defaultImpl/CameraService.h>

namespace cl{
    void CameraService::updateCameraViewMatrix(Camera *cameraPtr){
        CL_Vec3 location = cameraPtr->getLocation();
        CL_Vec3 lookatPoint = cameraPtr->getLookAtPoint();
        CL_Vec3 upVector = cameraPtr->getUpVector();
        cameraPtr->setViewMatrix(CL_lookAt(location, lookatPoint, upVector));
    }
    void CameraService::updateCameraProjectionMatrix(Camera *cameraPtr){
        float fov = cameraPtr->getFov();
        float aspect = cameraPtr->getAspect();
        float nearPlane = cameraPtr->getNearPlane();
        float farPlane = cameraPtr->getFarPlane();
        cameraPtr->setProjectionMatrix(CL_perspective(fov, aspect, nearPlane, farPlane));
    }
}