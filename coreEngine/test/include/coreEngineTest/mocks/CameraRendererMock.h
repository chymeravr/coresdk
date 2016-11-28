#ifndef ANDROIDSDK_CAMERARENDERERMOCK_H
#define ANDROIDSDK_CAMERARENDERERMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/model/ICameraRenderer.h>

namespace cl{
    class CameraRendererMock : public ICameraRenderer{
        MOCK_METHOD1(initialize, bool(Camera *cameraPtr));
        MOCK_METHOD1(draw, void(Camera *cameraPtr));
        MOCK_METHOD1(deinitialize, void(Camera *cameraPtr));
    };
}

#endif //ANDROIDSDK_CAMERARENDERERMOCK_H