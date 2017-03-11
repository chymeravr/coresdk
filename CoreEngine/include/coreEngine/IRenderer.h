#ifndef COREENGINE_IRENDERER_H
#define COREENGINE_IRENDERER_H

#include<coreEngine/renderObjects/Scene.h>

namespace cl{
	enum EYE
	{
		LEFT = 0,
		RIGHT = 1
	};

	class IRenderer{
	public:
		virtual bool start() = 0;
		virtual bool initialize(Scene *scene) = 0;
		virtual void update() = 0;
		virtual void draw(Scene *scene) = 0;							// draw common stuff - clear buffers etc. 
		virtual void draw(Scene *scene, EYE eye) = 0;					// draw left/right eye independently
		virtual void deinitialize(Scene *scene) = 0;
		virtual void stop() = 0;
	};
}

#endif //COREENGINE_IRENDERER_H