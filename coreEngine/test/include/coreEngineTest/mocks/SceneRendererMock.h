#ifndef ANDROID_SDK_SCENERENDERERMOCK_H
#define ANDROID_SDK_SCENERENDERERMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/model/ISceneRenderer.h>

namespace cl{
	class SceneRendererMock : public ISceneRenderer{
	public:
		MOCK_METHOD1(initialize, bool(Scene *scenePtr));
		MOCK_METHOD1(draw, void(Scene *scenePtr));
		MOCK_METHOD1(deinitialize, void(Scene *scenePtr));
	};
}

#endif //ANDROID_SDK_SCENERENDERERMOCK_H