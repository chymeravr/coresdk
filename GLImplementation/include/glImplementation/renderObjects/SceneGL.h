#ifndef GLIMPLEMENTATION_SCENEGL_H
#define GLIMPLEMENTATION_SCENEGL_H

#include <coreEngine/renderObjects/Scene.h>
#include <coreEngine/conf/Types.h>

namespace cl{
	class SceneGL : public Scene, public IRenderable{
	public:
		SceneGL(ILoggerFactory *loggerFactory, std::string id);
		IRenderable *getRenderable();
		void setBackgroundColor(CL_Vec4 color);
		void setDepthTest(bool enable);
		bool initialize();
		void draw();
		void deinitialize();
	private:
		std::unique_ptr<ILogger> logger;
		GLuint vertexArrayId;
		CL_Vec4 color;
		bool depthTest = false;
	};
}

#endif //GLIMPLEMENTATION_SCENEGL_H