#ifndef ANDROIDSDK_SHADERRENDERERMOCK_H
#define ANDROIDSDK_SHADERRENDERERMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/model/IShaderRenderer.h>

namespace cl{
    class ShaderRendererMock : public IShaderRenderer{
        MOCK_METHOD1(initialize, bool(Shader *modelPtr));
        MOCK_METHOD1(draw, void(Shader *modelPtr));
        MOCK_METHOD1(deinitialize, void(Shader *modelPtr));
    };
}

#endif //ANDROIDSDK_SHADERRENDERERMOCK_H