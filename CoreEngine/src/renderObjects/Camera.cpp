#include <coreEngine/renderObjects/Camera.h>

namespace cl{
	Camera::Camera(const std::string &sceneId, ILoggerFactory *loggerFactory) : Relation(loggerFactory){
		this->sceneId = sceneId;
		//this->createBiRelation(scene);
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
        // projection matrix calculations can be done for symetric and asymetric viewing frustum
		if(!this->isAsymetricProjection) {
            // symetric case
			if (farPlane > nearPlane) {
				projectionMatrix = CL_perspective(fov, aspect, nearPlane, farPlane);
			} else {
				projectionMatrix = CL_tweakedInfinitePerspective(fov, aspect, nearPlane);
			}
		}else{
            // asymetric case - more general
			if (farPlane > nearPlane) {
				projectionMatrix = CL_frustum(left, right, bottom, top, nearPlane, farPlane);
			} else {
				// todo: does this case arise?
				//projectionMatrix = CL_infiniteFrustum(left, right, top, bottom, nearPlane);
			}
		}
	}

    void Camera::setIsAsymetricProjection(bool isAsymetric) {
        this->isAsymetricProjection = isAsymetric;
    }

    void Camera::setLeft(const float &left){
        this->left = left;
    }

    void Camera::setRight(const float &right){
        this->right = right;
    }

    void Camera::setTop(const float &top){
        this->top = top;
    }

    void Camera::setBottom(const float &bottom){
        this->bottom = bottom;
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