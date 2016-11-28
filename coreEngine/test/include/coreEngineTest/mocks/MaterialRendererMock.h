#ifndef ANDROIDSDK_MATERIALRENDERERMOCK_H
#define ANDROIDSDK_MATERIALRENDERERMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/model/IMaterialRenderer.h>

namespace cl{
    class MaterialRendererMock : public IMaterialRenderer{
        MOCK_METHOD1(initialize, bool(Material *modelPtr));
        MOCK_METHOD1(draw, void(Material *modelPtr));
        MOCK_METHOD1(deinitialize, void(Material *modelPtr));
    };
}

#endif //ANDROIDSDK_MATERIALRENDERERMOCK_H