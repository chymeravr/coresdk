#ifndef COREENGINE_TRANSFORMCAMERA_H
#define COREENGINE_TRANSFORMCAMERA_H

#include <coreEngine/renderObjects/Camera.h>
#include <coreEngine/components/transform/Transform.h>
#include <coreEngine/renderObjects/IRenderable.h>

namespace cl{
	class TransformCamera : public Transform, public IRenderable{
	public:
		TransformCamera(Camera *camera, ILoggerFactory *loggerFactory);
		virtual IRenderable *getRenderable();
		virtual bool initialize();
		virtual void draw();
		virtual void deinitialize();
	private:
		Camera *camera = nullptr;
		std::unique_ptr<ILogger> logger;
	};
}

#endif //COREENGINE_TRANSFORMCAMERA_H