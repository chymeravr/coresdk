#ifndef COREENGINE_TRANSFORMTREECAMERA_H
#define COREENGINE_TRANSFORMTREECAMERA_H

#include <coreEngine/components/transformTree/TransformTree.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <coreEngine/renderObjects/Camera.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <coreEngine/util/ILogger.h>
#include <coreEngine/renderObjects/IComponent.h>

namespace cl{
	class TransformTreeCamera : public TransformTree, public IRenderable, public IComponent{
	public:
		TransformTreeCamera(Camera *camera, ILoggerFactory *loggerFactory);
		std::string getComponentType();
		virtual IRenderable *getRenderable();
		virtual bool initialize();
		virtual void draw();
		virtual void deinitialize();
	private:
		std::string componentType = "transformTree";
		Camera *camera = nullptr;
		std::unique_ptr<ILogger> logger;
	};
}

#endif //COREENGINE_TRANSFORMTREECAMERA_H