//
// Created by chimeralabs on 11/2/2016.
//

#include <coreEngine/components/defaultImpl/Transform.h>
#include <coreEngine/util/LoggerFactory.h>
#include <coreEngine/service/IServicePool.h>

namespace cl{

    Transform::Transform() {
        type = "transform";
        loggerPtr = &LoggerFactory::getInstance().getLoggerInstance("app::components::Transform");
        modelServicePtr = IServicePool::getInstance().getIModelService();
        materialServicePtr = IServicePool::getInstance().getIMaterialService();
        shaderServicePtr = IServicePool::getInstance().getIShaderService();
        sceneServicePtr = IServicePool::getInstance().getISceneService();
    }

    std::string Transform::getComponentType() {
        return type;
    }

    void Transform::initialize() {

    }

    void Transform::draw() {
        if(objectPtr->getObjectType() == "model") {
            std::vector<Object*>& cameras = sceneServicePtr->getObjectsByObjectType(*scenePtr, "camera");
            if(cameras.size()!=1){
                loggerPtr->log(LOG_ERROR, "cameras number is not one. Returning.");
            }
            Camera &camera = (Camera&) *cameras[0];

            CL_Mat44 Rx = CL_RotationMatrixX(rotation[0]);
            CL_Mat44 Ry = CL_RotationMatrixY(rotation[1]);
            CL_Mat44 Rz = CL_RotationMatrixZ(rotation[2]);
            CL_Mat44 T = CL_TranlationMatrix(position);
            CL_Mat44 S = CL_ScaleMatrix(scale);
            CL_Mat44 modelMatrix = T * Ry * Rx * Rz * S;
            camera.setModelMatrix(modelMatrix);
        }
        else if(objectPtr->getObjectType() == "camera"){
            Camera &camera = (Camera &)(*objectPtr);
            camera.setLocation(position);
            CL_Mat44 Rx = CL_RotationMatrixX(rotation[0]);
            CL_Mat44 Ry = CL_RotationMatrixY(rotation[1]);
            CL_Mat44 Rz = CL_RotationMatrixZ(rotation[2]);
            CL_Mat44 T = CL_TranlationMatrix(position);
            CL_Mat44 S = CL_ScaleMatrix(scale);
            CL_Mat44 rot = Ry * Rx * Rz;
            CL_Mat44 modelMatrix = T * rot;
            CL_Vec3 localLookAt = camera.getLocalLookAtPoint();
            CL_Vec4 localLookAt4v = CL_Vec4(localLookAt[0], localLookAt[1], localLookAt[2], 1.0);
            CL_Vec4 lookAtPoint = modelMatrix * localLookAt4v;
            CL_Vec3 lookAt3v (lookAtPoint[0], lookAtPoint[1], lookAtPoint[2]);
            CL_Vec3 localUp = camera.getLocalUpVector();
            CL_Vec4 localUp4v = CL_Vec4(localUp[0], localUp[1], localUp[2], 0.0); //vector in homogeneous coordinate
            CL_Vec4 up = rot * localUp4v;
            CL_Vec3 up3v (up[0], up[1], up[2]);
            camera.setLookAtPoint(lookAt3v);
            camera.setUpVector(up3v);
        }
    }

    void Transform::deinitialize() {

    }

    void Transform::setObject(Object &object) {
        objectPtr = &object;
    }

    Object &Transform::getObject() {
        return *objectPtr;
    }

    void Transform::setScene(Scene &scene) {
        this->scenePtr = &scene;
    }

    void Transform::setPosition(CL_Vec3 position) {
        this->position = position;
    }

    CL_Vec3 Transform::getPosition() {
        return position;
    }

    void Transform::setRoation(CL_Vec3 rotation) {
        this->rotation = rotation;
    }

    CL_Vec3 Transform::getRotation() {
        return rotation;
    }

    void Transform::setScale(CL_Vec3 scale) {
        this->scale = scale;
    }

    CL_Vec3 Transform::getScale() {
        return scale;
    }

}