#ifndef COREENGINETEST_SCENEMOCK_H
#define COREENGINETEST_SCENEMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/renderObjects/Scene.h>

namespace cl{
	class SceneMock : public Scene{
	public:
		SceneMock(ILoggerFactory *loggerFactory, std::string id) : Scene(loggerFactory, id){}
		MOCK_METHOD0(getRenderable, IRenderable* (void));
		MOCK_METHOD1(setBackgroundColor, void(CL_Vec4));
		MOCK_METHOD1(setDepthTest, void(bool));
	};
}

#endif //COREENGINETEST_SCENEMOCK_H