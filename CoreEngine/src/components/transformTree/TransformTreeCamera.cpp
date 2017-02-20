#include <coreEngine/components/transformTree/TransformTreeCamera.h>
#include <assert.h>

namespace cl{
	TransformTreeCamera::TransformTreeCamera(Camera *camera, ILoggerFactory *loggerFactory){
		assert(camera != nullptr);
		assert(loggerFactory != nullptr);
		logger = loggerFactory->createLogger("coreEngine::TransformTreeCamera: ");
		this->camera = camera;
	}
	std::string TransformTreeCamera::getComponentType(){
		return componentType;
	}
	IRenderable *TransformTreeCamera::getRenderable(){
		return this;
	}
	bool TransformTreeCamera::initialize(){
		return true;
	}
	void TransformTreeCamera::draw(){
		if (!isDirty)
			return;
		CL_Mat44 modelMatrix = getGlobalTransform();
		CL_Vec3 localLocation = camera->getLocalLocation();
		CL_Vec4 localLocation4v = CL_Vec4(localLocation[0], localLocation[1], localLocation[2], 1.0);
		CL_Vec4 location = modelMatrix * localLocation4v;
		camera->setLocation(CL_Vec3(location[0], location[1], location[2]));
		CL_Vec3 localLookAt = camera->getLocalLookAtPoint();
		CL_Vec4 localLookAt4v = CL_Vec4(localLookAt[0], localLookAt[1], localLookAt[2], 1.0);
		CL_Vec4 lookAtPoint = modelMatrix * localLookAt4v;
		CL_Vec3 lookAt3v(lookAtPoint[0], lookAtPoint[1], lookAtPoint[2]);
		CL_Vec3 localUp = camera->getLocalUpVector();
		CL_Vec4 localUp4v = CL_Vec4(localUp[0], localUp[1], localUp[2], 0.0); //vector in homogeneous coordinate
		CL_Vec4 up = modelMatrix * localUp4v;
		CL_Vec3 up3v(up[0], up[1], up[2]);
		camera->setLookAtPoint(lookAt3v);
		camera->setUpVector(up3v);
	}
	void TransformTreeCamera::deinitialize(){
	}
}