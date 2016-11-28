#ifndef ANDROIDSDK_MATERIALDIFFUSETEXTURESERVICE_H
#define ANDROIDSDK_MATERIALDIFFUSETEXTURESERVICE_H

#include<memory>
#include<coreEngine/service/IMaterialDiffuseTextureService.h>
#include<coreEngine/util/ILogger.h>
#include<coreEngine/util/ILoggerFactory.h>

namespace cl{
    class MaterialDiffuseTextureService : public IMaterialDiffuseTextureService{
    private:
        std::unique_ptr<ILogger> loggerPtr;
    public:
        MaterialDiffuseTextureService(ILoggerFactory *loggerFactoryPtr);
        void addDiffuseTexture(MaterialDiffuseTexture *materialPtr, Texture *texturePtr);
        void removeDiffuseTexture(MaterialDiffuseTexture *materialPtr);
        Texture *getDiffuseTexture(MaterialDiffuseTexture *materialPtr);
    };
}

#endif //ANDROIDSDK_MATERIALDIFFUSETEXTURESERVICE_H