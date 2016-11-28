//
// Created by chimeralabs on 10/19/2016.
//

#ifndef ANDROIDSDK_CAMERA_H
#define ANDROIDSDK_CAMERA_H

#include <coreEngine/conf/Types.h>
#include <coreEngine/model/Object.h>
#include <coreEngine/model/ICameraRenderer.h>

namespace cl{
    class Camera : public Object{
    protected:
        CL_Mat44 projectionMatrix;
        CL_Mat44 viewMatrix;
        CL_Mat44 modelMatrix;
        CL_Vec3 location;
        CL_Vec3 lookAtPoint;
        CL_Vec3 upVector;
        CL_Vec3 localLocation = CL_Vec3(0.0, 0.0, 0.0);
        CL_Vec3 localLookAtPoint = CL_Vec3(0.0, 0.0, -1.0);
        CL_Vec3 localUpVector = CL_Vec3(0.0, 1.0, 0.0);
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
        std::unique_ptr<ICameraRenderer> cameraRendererPtr;

    public:
        Camera(std::string &tag, std::unique_ptr<ICameraRenderer> cameraRendererPtr) : Object("camera", tag) { 
            this->cameraRendererPtr = std::move(cameraRendererPtr);
        }

        const CL_Mat44 &getProjectionMatrix() const {
            return projectionMatrix;
        }

        void setProjectionMatrix(const CL_Mat44 &projectionMatrix) {
            Camera::projectionMatrix = projectionMatrix;
        }

        const CL_Mat44 &getViewMatrix() const {
            return viewMatrix;
        }

        void setViewMatrix(const CL_Mat44 &viewMatrix) {
            Camera::viewMatrix = viewMatrix;
        }

        const CL_Mat44 &getModelMatrix() const {
            return modelMatrix;
        }

        void setModelMatrix(const CL_Mat44 &modelMatrix) {
            Camera::modelMatrix = modelMatrix;
        }

        const CL_Vec3 &getLocation() const {
            return location;
        }

        void setLocation(const CL_Vec3 &location) {
            Camera::location = location;
        }

        const CL_Vec3 &getLookAtPoint() const {
            return lookAtPoint;
        }

        void setLookAtPoint(const CL_Vec3 &lookAtPoint) {
            Camera::lookAtPoint = lookAtPoint;
        }

        const CL_Vec3 &getUpVector() const {
            return upVector;
        }

        void setUpVector(const CL_Vec3 &upVector) {
            Camera::upVector = upVector;
        }

        const CL_Vec3 &getLocalLocation() const {
            return localLocation;
        }

        void setLocalLocation(const CL_Vec3 &localLocation) {
            Camera::localLocation = localLocation;
        }

        const CL_Vec3 &getLocalLookAtPoint() const {
            return localLookAtPoint;
        }

        void setLocalLookAtPoint(const CL_Vec3 &localLookAtPoint) {
            Camera::localLookAtPoint = localLookAtPoint;
        }

        const CL_Vec3 &getLocalUpVector() const {
            return localUpVector;
        }

        void setLocalUpVector(const CL_Vec3 &localUpVector) {
            Camera::localUpVector = localUpVector;
        }

        /**
        * fov is in radians. It represents vertical field of view from top to down.
        */
        float getFov() const {
            return fov;
        }

        /**
        * fov is in radians. It represents vertical field of view from top to down.
        */
        void setFov(float fov) {
            Camera::fov = fov;
        }

        /**
        * aspect ratio = width/height
        */
        float getAspect() const {
            return aspect;
        }

        /**
        * aspect ratio = width/height
        */
        void setAspect(float aspect) {
            Camera::aspect = aspect;
        }

        float getNearPlane() const {
            return nearPlane;
        }

        void setNearPlane(float nearPlane) {
            Camera::nearPlane = nearPlane;
        }

        float getFarPlane() const {
            return farPlane;
        }

        void setFarPlane(float farPlane) {
            Camera::farPlane = farPlane;
        }

        ICameraRenderer *getCameraRendererPtr(){
            return cameraRendererPtr.get();
        }
    };
}

#endif //ANDROIDSDK_CAMERA_H
