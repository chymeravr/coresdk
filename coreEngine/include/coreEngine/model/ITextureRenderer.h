#ifndef ANDROIDSDK_ITEXTURERENDERER_H
#define ANDROIDSDK_ITEXTURERENDERER_H

namespace cl{
    class Texture;
    class ITextureRenderer{
        virtual bool initialize(Texture *materialPtr) = 0;
        virtual void draw(Texture *materialPtr) = 0;
        virtual void deinitialize(Texture *materialPtr) = 0;
    };
}

#endif //ANDROIDSDK_ITEXTURERENDERER_H