#include <coreEngine/components/transform/TransformCamera.h>
#include <assert.h>

namespace cl{
	TransformCamera::TransformCamera(Camera *camera, ILoggerFactory *loggerFactory){
		assert(camera != nullptr);
		assert(loggerFactory != nullptr);
		logger = loggerFactory->createLogger("coreEngine::TransformCamera: ");
		this->camera = camera;
	}
	IRenderable *TransformCamera::getRenderable(){
		return this;
	}
	bool TransformCamera::initialize(){
		return true;
	}
	void TransformCamera::draw(){
		camera->setLocation(position);
		CL_Mat44 Rx = CL_RotationMatrixX(rotation[0] * CL_PI / 180);
		CL_Mat44 Ry = CL_RotationMatrixY(rotation[1] * CL_PI / 180);
		CL_Mat44 Rz = CL_RotationMatrixZ(rotation[2] * CL_PI / 180);
		CL_Mat44 T = CL_TranlationMatrix(position);
		CL_Mat44 S = CL_ScaleMatrix(scale);
		CL_Mat44 rot = Ry * Rx * Rz;
		CL_Mat44 modelMatrix = T * rot;
		CL_Vec3 localLookAt = camera->getLocalLookAtPoint();
		CL_Vec4 localLookAt4v = CL_Vec4(localLookAt[0], localLookAt[1], localLookAt[2], 1.0);
		CL_Vec4 lookAtPoint = modelMatrix * localLookAt4v;
		CL_Vec3 lookAt3v(lookAtPoint[0], lookAtPoint[1], lookAtPoint[2]);
		CL_Vec3 localUp = camera->getLocalUpVector();
		CL_Vec4 localUp4v = CL_Vec4(localUp[0], localUp[1], localUp[2], 0.0); //vector in homogeneous coordinate
		CL_Vec4 up = rot * localUp4v;
		CL_Vec3 up3v(up[0], up[1], up[2]);
		camera->setLookAtPoint(lookAt3v);
		camera->setUpVector(up3v);
	}
	void TransformCamera::deinitialize(){
	}
}