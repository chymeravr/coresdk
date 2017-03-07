#include <coreEngine/renderObjects/Camera.h>

namespace cl{
	Camera::Camera(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene) : Relation(loggerFactory){
		this->sceneId = sceneId;
		this->createBiRelation(scene);
	}

	std::string Camera::getSceneId(){
		return this->sceneId;
	}

	std::string Camera::getType(){
		return this->type;
	}

	std::string Camera::getUniqueIdentifier(){
		return this->sceneId;
	}

	void Camera::calculateViewMatrix(){
		viewMatrix = CL_lookAt(location, lookAtPoint, upVector);
	}

	void Camera::calculateProjectionMatrix(){
		if(farPlane > nearPlane){
			projectionMatrix = CL_perspective(fov, aspect, nearPlane, farPlane);
		}else{
			projectionMatrix = CL_tweakedInfinitePerspective(fov, aspect, nearPlane);
		}
	}

	CL_Mat44 Camera::getViewMatrix(){
		return this->viewMatrix;
	}

	CL_Mat44 Camera::getProjectionMatrix(){
		return this->projectionMatrix;
	}

	void Camera::setLocation(const CL_Vec3 &location){
		this->location = location;
	}

	void Camera::setLookAtPoint(const CL_Vec3 &lookAtPoint){
		this->lookAtPoint = lookAtPoint;
	}

	void Camera::setUpVector(const CL_Vec3 &upVector){
		this->upVector = upVector;
	}

	void Camera::setFov(const float &fov){
		this->fov = fov;
	}

	void Camera::setAspect(const float &aspect){
		this->aspect = aspect;
	}

	void Camera::setNearPlane(const float &nearPlane){
		this->nearPlane = nearPlane;
	}

	void Camera::setFarPlane(const float &farPlane){
		this->farPlane = farPlane;
	}

	CL_Vec3 Camera::getLocalLookAtPoint(){
		return localLookAtPoint;
	}

	CL_Vec3 Camera::getLocalUpVector(){
		return localUpVector;
	}

	CL_Vec3 Camera::getLocalLocation(){
		return localLocation;
	}
}