#ifndef ANDROIDSDK_IMATERIALDIFFUSETEXTURESERVICE_H
#define ANDROIDSDK_IMATERIALDIFFUSETEXTURESERVICE_H

#include <coreEngine/model/MaterialDiffuseTexture.h>
#include <coreEngine/model/Texture.h>

namespace cl{
    class IMaterialDiffuseTextureService{
    public:
        virtual void addDiffuseTexture(MaterialDiffuseTexture *materialPtr, Texture *texturePtr) = 0;
        virtual void removeDiffuseTexture(MaterialDiffuseTexture *materialPtr) = 0;
        virtual Texture *getDiffuseTexture(MaterialDiffuseTexture *materialPtr) = 0;
    };
}

#endif //ANDROIDSDK_IMATERIALDIFFUSETEXTURESERVICE_H