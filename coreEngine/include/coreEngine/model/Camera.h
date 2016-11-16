//
// Created by chimeralabs on 10/19/2016.
//

#ifndef ANDROIDSDK_CAMERA_H
#define ANDROIDSDK_CAMERA_H

#include <coreEngine/util/Logger.h>
#include <coreEngine/conf/Types.h>
#include <coreEngine/model/Object.h>

namespace cl{
    class Camera : public Object{
    protected:
        CL_Mat44 projectionMatrix;
        CL_Mat44 viewMatrix;
        CL_Mat44 modelMatrix;
        CL_Mat44 mvpMatrix;
        CL_Vec3 location;
        CL_Vec3 lookAtPoint;
        CL_Vec3 upVector;
        CL_Vec3 localLocation = CL_Vec3(0.0, 0.0, 0.0);
        CL_Vec3 localLookAtPoint = CL_Vec3(0.0, 0.0, -1.0);
        CL_Vec3 localUpVector = CL_Vec3(0.0, 1.0, 0.0);
        float fov;
        float aspect;
        float nearPlane;
        float farPlane;
    public:
        Camera(std::string &tag) : Object("camera", tag) { }

        CL_Mat44 &getProjectionMatrix() {
            return projectionMatrix;
        }

        void setProjectionMatrix(CL_Mat44 &projectionMatrix) {
            Camera::projectionMatrix = projectionMatrix;
        }

        CL_Mat44 &getViewMatrix() {
            return viewMatrix;
        }

        void setViewMatrix(CL_Mat44 &viewMatrix) {
            Camera::viewMatrix = viewMatrix;
        }

        CL_Mat44 &getModelMatrix() {
            return modelMatrix;
        }

        void setModelMatrix(CL_Mat44 &modelMatrix) {
            Camera::modelMatrix = modelMatrix;
        }

        CL_Mat44 &getMvpMatrix() {
            return mvpMatrix;
        }

        void setMvpMatrix(CL_Mat44 &mvpMatrix) {
            Camera::mvpMatrix = mvpMatrix;
        }

        CL_Vec3 &getLocation() {
            return location;
        }

        void setLocation(CL_Vec3 &location) {
            Camera::location = location;
        }

        CL_Vec3 &getLookAtPoint() {
            return lookAtPoint;
        }

        void setLookAtPoint(CL_Vec3 &lookAtPoint) {
            Camera::lookAtPoint = lookAtPoint;
        }

        CL_Vec3 &getUpVector() {
            return upVector;
        }

        void setUpVector(CL_Vec3 &upVector) {
            Camera::upVector = upVector;
        }

        float getFov() {
            return fov;
        }

        void setFov(float fov) {
            Camera::fov = fov;
        }

        float getAspect() {
            return aspect;
        }

        void setAspect(float aspect) {
            Camera::aspect = aspect;
        }

        float getNearPlane() {
            return nearPlane;
        }

        void setNearPlane(float nearPlane) {
            Camera::nearPlane = nearPlane;
        }

        float getFarPlane() {
            return farPlane;
        }

        void setFarPlane(float farPlane) {
            Camera::farPlane = farPlane;
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
    };
}

#endif //ANDROIDSDK_CAMERA_H
