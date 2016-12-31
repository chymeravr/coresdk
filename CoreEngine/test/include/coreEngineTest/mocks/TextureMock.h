#ifndef COREENGINETEST_TEXTUREMOCK_H
#define COREENGINETEST_TEXTUREMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/renderObjects/Texture.h>

namespace cl{
	class TextureMock : public Texture{
	public:
		TextureMock(const std::string &sceneId, ILoggerFactory *loggerFactory) :Texture(sceneId, loggerFactory){}
		MOCK_METHOD0(getRenderable, IRenderable *(void));
	};
}

#endif //COREENGINETEST_TEXTUREMOCK_H