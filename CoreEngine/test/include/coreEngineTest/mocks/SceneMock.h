#ifndef COREENGINETEST_SCENEMOCK_H
#define COREENGINETEST_SCENEMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/renderObjects/Scene.h>

namespace cl{
    class SceneMock : public Scene{
    public:
        SceneMock(ILoggerFactory *loggerFactory) : Scene(loggerFactory){}
        MOCK_METHOD0(getRenderable, IRenderable* (void));
    };

}

#endif //COREENGINETEST_SCENEMOCK_H