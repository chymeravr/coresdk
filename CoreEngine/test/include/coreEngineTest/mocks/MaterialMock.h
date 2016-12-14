#ifndef COREENGINETEST_MATERIALMOCK_H
#define COREENGINETEST_MATERIALMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/renderObjects/Material.h>

namespace cl{
    class MaterialMock : public Material{
    public:
        MaterialMock(const std::string &sceneId, Shader *shader, ILoggerFactory *loggerFactory) :Material(sceneId, shader, loggerFactory){}
        MOCK_METHOD0(getRenderable, IRenderable *(void));
    };
}

#endif //COREENGINETEST_MATERIALMOCK_H