#ifndef RENDERER_RENDERERNOHMDSTEREO_H
#define RENDERER_RENDERERNOHMDSTEREO_H

#include <coreEngine/IRenderer.h>
#include <renderer/RendererNoHMD.h>

namespace cl{
	class RendererNoHMDStereo : public RendererNoHMD{
	public:
		void drawInit(Scene *scene);
		void draw(Scene *scene, EYE eye);
		void drawComplete();
	};
}

#endif //RENDERER_RENDERERNOHMD_H