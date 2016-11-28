#include <coreEngine/service/defaultImpl/MaterialDiffuseTextureService.h>
#include <assert.h>

namespace cl{
    MaterialDiffuseTextureService::MaterialDiffuseTextureService(ILoggerFactory *loggerFactoryPtr){
        assert(loggerFactoryPtr != nullptr);
        loggerPtr = loggerFactoryPtr->createLogger("app::coreEngine::MaterialDiffuseTextureService:");
    }

    void MaterialDiffuseTextureService::addDiffuseTexture(MaterialDiffuseTexture *materialPtr, Texture *texturePtr){
        assert(materialPtr != nullptr);
        assert(texturePtr != nullptr);

        Texture *texture = materialPtr->getDiffuseTexture();
        if (texture != nullptr){
            removeDiffuseTexture(materialPtr);
        }
        materialPtr->setDiffuseTexture(texturePtr);
        loggerPtr->log(LOG_INFO, "Texture:" + texturePtr->getTag() + " added to Material:" + materialPtr->getTag());
    }
    void MaterialDiffuseTextureService::removeDiffuseTexture(MaterialDiffuseTexture *materialPtr){
        assert(materialPtr != nullptr);
        Texture *texturePtr = materialPtr->getDiffuseTexture();
        if (texturePtr != nullptr){
            materialPtr->setDiffuseTexture(nullptr);
            loggerPtr->log(LOG_INFO, "Texture:" + texturePtr->getTag() + " removed from Material:" + materialPtr->getTag());
        }
    }
    Texture *MaterialDiffuseTextureService::getDiffuseTexture(MaterialDiffuseTexture *materialPtr){
        assert(materialPtr != nullptr);
        return materialPtr->getDiffuseTexture();
    }
}