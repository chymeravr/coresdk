#ifndef COREENGINETEST_CAMERAMOCK_H
#define COREENGINETEST_CAMERAMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/renderObjects/Camera.h>

namespace cl{
	class CameraMock : public Camera{
	public:
		CameraMock(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene) :Camera(sceneId, loggerFactory, scene){}
		MOCK_METHOD0(getRenderable, IRenderable *(void));
	};
}

#endif //COREENGINETEST_CAMERAMOCK_H