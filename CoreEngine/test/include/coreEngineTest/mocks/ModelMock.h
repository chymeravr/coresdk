#ifndef COREENGINETEST_MODELMOCK_H
#define COREENGINETEST_MODELMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/renderObjects/Model.h>
#include <coreEngineTest/mocks/ComponentMock.h>

namespace cl{
	class ModelMock : public Model{
	public:
		ModelMock(const std::string &sceneId, ILoggerFactory *loggerFactory) :Model(sceneId, loggerFactory){}
		MOCK_METHOD0(getRenderable, IRenderable *(void));
	};
}

#endif //COREENGINETEST_MODELMOCK_H