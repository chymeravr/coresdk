#ifndef GLIMPLEMENTATION_CAMERAGLCONTAINER_H
#define GLIMPLEMENTATION_CAMERAGLCONTAINER_H

#include <memory>
#include <coreEngine/renderObjects/IRenderable.h>
#include <glImplementation/renderObjects/CameraGL.h>
#include <conf/GLTypes.h>

namespace cl{
	class CameraGLContainer : public IRenderable{
	public:
		CameraGLContainer(CL_GLuint &viewMatrixId, CL_GLuint &projectionMatrixId, CameraGL *cameraGL, ILogger *logger);
		bool initialize();
		void draw();
		void deinitialize();

	private:
		CL_GLuint viewMatrixId;
		CL_GLuint projectionMatrixId;
		CameraGL *cameraGL;
		ILogger *logger;
	};
}

#endif //GLIMPLEMENTATION_CAMERAGLCONTAINER_H