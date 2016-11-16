//
// Created by chimeralabs on 10/26/2016.
//

#include <coreEngine/service/defaultImpl/CameraService.h>

namespace cl{
    void CameraService::updateCamera(Camera &camera){
        CL_Mat44 &viewMatrix = camera.getViewMatrix();
        CL_Mat44 &projectionMatrix = camera.getProjectionMatrix();
        CL_Mat44 &mvpMatrix = camera.getMvpMatrix();
        viewMatrix = CL_lookAt(camera.getLocation(), camera.getLookAtPoint(), camera.getUpVector());
        float fov = camera.getFov();
        float aspect = camera.getAspect();
        float nearPlane = camera.getNearPlane();
        float farPlane = camera.getFarPlane();
        projectionMatrix = CL_perspective(fov, aspect, nearPlane, farPlane);
        mvpMatrix = projectionMatrix * viewMatrix * camera.getModelMatrix();
    }
}