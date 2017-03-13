#ifndef RENDERER_RENDERERNOHMDSTEREO_H
#define RENDERER_RENDERERNOHMDSTEREO_H

#include <coreEngine/IRenderer.h>
#include <renderer/RendererNoHMD.h>

namespace cl{
	class RendererNoHMDStereo : public RendererNoHMD{
	public:
		void draw(Scene *scene);
		void draw(Scene *scene, EYE eye);
	};
}

#endif //RENDERER_RENDERERNOHMD_H