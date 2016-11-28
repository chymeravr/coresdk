//
// Created by chimeralabs on 10/27/2016.
//

#include <coreEngine/service/defaultImpl/ShaderService.h>
#include <coreEngine/service/IServicePool.h>

namespace cl{
    ShaderService::ShaderService(ILoggerFactory *loggerFactoryPtr, std::unique_ptr<IObjectService> objectServicePtr) {
        assert(objectServicePtr != nullptr);
        assert(loggerFactoryPtr != nullptr);
        loggerPtr = loggerFactoryPtr->createLogger("app::coreEngine::ShaderService");
        this->objectServicePtr = std::move(objectServicePtr);
    }

    std::vector<Material *> ShaderService::getMaterials(Shader *shaderPtr) {
        assert(shaderPtr != nullptr);
        std::vector<Object *> objects = objectServicePtr->getLinkedObjectsByObjectType(shaderPtr, "material");
        std::vector<Material *> materials;
        for(auto it=objects.cbegin(); it!=objects.cend(); it++){
            materials.push_back((Material*)(*it));
        }
        return materials;
    }

}