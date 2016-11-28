#ifndef ANDROIDSDK_IMATERIALRENDERER_H
#define ANDROIDSDK_IMATERIALRENDERER_H

namespace cl{
    class Material;
    class IMaterialRenderer{
        virtual bool initialize(Material *materialPtr) = 0;
        virtual void draw(Material *materialPtr) = 0;
        virtual void deinitialize(Material *materialPtr) = 0;
    };
}

#endif //ANDROIDSDK_IMATERIALRENDERER_H