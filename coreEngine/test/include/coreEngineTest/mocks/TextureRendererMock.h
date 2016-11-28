#ifndef ANDROIDSDK_TEXTURERENDERERMOCK_H
#define ANDROIDSDK_TEXTURERENDERERMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/model/ITextureRenderer.h>

namespace cl{
    class TextureRendererMock : public ITextureRenderer{
        MOCK_METHOD1(initialize, bool(Texture *texturePtr));
        MOCK_METHOD1(draw, void(Texture *texturePtr));
        MOCK_METHOD1(deinitialize, void(Texture *texturePtr));
    };
}

#endif //ANDROIDSDK_TEXTURERENDERERMOCK_H