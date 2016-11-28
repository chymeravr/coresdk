//
// Created by chimeralabs on 10/26/2016.
//

#include <coreEngine/service/defaultImpl/MaterialService.h>
#include <coreEngine/service/IServicePool.h>

namespace cl{

    MaterialService::MaterialService(ILoggerFactory *loggerFactoryPtr, std::unique_ptr<IObjectService> objectServicePtr) {
        assert(loggerFactoryPtr != nullptr);
        assert(objectServicePtr != nullptr);
        loggerPtr = loggerFactoryPtr->createLogger("app::coreEngine::MaterialService:");
        this->objectServicePtr = std::move(objectServicePtr);
    }

    void MaterialService::addMaterialToModel(Material *materialPtr, Model *modelPtr) {
        assert(materialPtr != nullptr);
        assert(modelPtr != nullptr);
        Object *objectPtr = objectServicePtr->getLinkedObjectByTag(materialPtr, modelPtr->getTag());
        if(objectPtr==nullptr){
            objectServicePtr->linkObject(materialPtr, modelPtr);
            loggerPtr->log(LOG_INFO, "Model:" + modelPtr->getTag() + " added Material:"+materialPtr->getTag());
        }else{
            loggerPtr->log(LOG_WARN, "Material:" + materialPtr->getTag() + " already attached to Model:"+modelPtr->getTag());
        }
    }

    std::vector<Model *> MaterialService::getModelsOfMaterial(Material* materialPtr) {
        assert(materialPtr != nullptr);
        std::vector<Object*> objects = objectServicePtr->getLinkedObjectsByObjectType(materialPtr, "model");
        std::vector<Model *> models;
        for(auto it=objects.cbegin(); it!= objects.cend(); it++){
            models.push_back((Model*)(*it));
        }
        return models;
    }

    void MaterialService::removeMaterialFromModel(Material *materialPtr, Model *modelPtr) {
        assert(materialPtr != nullptr);
        assert(modelPtr != nullptr);
        Object *object = objectServicePtr->getLinkedObjectByTag(materialPtr, modelPtr->getTag());
        if (object == nullptr){
            return;
        }
        objectServicePtr->unlinkObject(materialPtr, modelPtr);
        loggerPtr->log(LOG_WARN, "Material:" + materialPtr->getTag() + " detached from Model:" + modelPtr->getTag());
    }

    Shader* MaterialService::getShader(Material *materialPtr) {
        assert(materialPtr != nullptr);
        std::vector<Object *> objects = objectServicePtr->getLinkedObjectsByObjectType(materialPtr, "shader");
        assert(objects.size() == 1);
        return (Shader*)objects[0];
    }
}