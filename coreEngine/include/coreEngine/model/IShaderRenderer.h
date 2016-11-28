#ifndef ANDROIDSDK_ISHADERRENDERER_H
#define ANDROIDSDK_ISHADERRENDERER_H

namespace cl{
    class Shader;
    class IShaderRenderer{
        virtual bool initialize(Shader *materialPtr) = 0;
        virtual void draw(Shader *materialPtr) = 0;
        virtual void deinitialize(Shader *materialPtr) = 0;
    };
}

#endif //ANDROIDSDK_ISHADERRENDERER_H