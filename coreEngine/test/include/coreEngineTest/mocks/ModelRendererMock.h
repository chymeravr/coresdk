#ifndef ANDROIDSDK_MODELRENDERERMOCK_H
#define ANDROIDSDK_MODELRENDERERMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/model/IModelRenderer.h>

namespace cl{
    class ModelRendererMock : public IModelRenderer{
        MOCK_METHOD1(initialize, bool(Model *modelPtr));
        MOCK_METHOD1(draw, void(Model *modelPtr));
        MOCK_METHOD1(deinitialize, void(Model *modelPtr));
    };
}

#endif //ANDROIDSDK_MODELRENDERERMOCK_H