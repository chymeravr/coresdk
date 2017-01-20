#ifndef COREENGINETEST_SHADERMOCK_H
#define COREENGINETEST_SHADERMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/renderObjects/Shader.h>

namespace cl{
	class ShaderMock : public Shader{
	public:
		ShaderMock(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene) :Shader(sceneId, loggerFactory, scene){}
		MOCK_METHOD0(getRenderable, IRenderable *(void));
	};
}

#endif //COREENGINETEST_SHADERMOCK_H