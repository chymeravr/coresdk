#ifndef ANDROIDSDK_ISCENERENDERER_H
#define ANDROIDSDK_ISCENERENDERER_H

namespace cl{
    class Scene;
	class ISceneRenderer{
	public:
		virtual bool initialize(Scene *scenePtr) = 0;
		virtual void draw(Scene *scenePtr) = 0;
		virtual void deinitialize(Scene *scenePtr) = 0;
	};
}

#endif //ANDROIDSDK_ISCENERENDERER_H