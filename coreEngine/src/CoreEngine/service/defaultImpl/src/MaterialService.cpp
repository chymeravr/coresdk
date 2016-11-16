//
// Created by chimeralabs on 10/26/2016.
//

#include <coreEngine/service/defaultImpl/MaterialService.h>
#include <coreEngine/service/IServicePool.h>

namespace cl{

    MaterialService::MaterialService() {
        loggerPtr = &LoggerFactory::getInstance().getLoggerInstance("app::coreEngine::MaterialService:");
        objectServicePtr = IServicePool::getInstance().getIObjectService();
    }

    void MaterialService::addMaterialToModel(Material &material, Model &model) {
        std::pair<bool, Object&>  object = objectServicePtr->getLinkedObjectByTag(material, model.getTag());
        if(!object.first){
            objectServicePtr->linkObject(material, model);
            loggerPtr->log(LOG_INFO, "Model:" + model.getTag() + " added Material:"+material.getTag());
        }else{
            loggerPtr->log(LOG_WARN, "Material:" + material.getTag() + " already attached to Model:"+model.getTag());
        }
    }

    std::vector<Model *> MaterialService::getModelsOfMaterial(Material &material) {
        std::pair<bool, std::vector<Object *>& > modelObjectsPair = objectServicePtr->getLinkedObjectsByObjectType(material, "model");
        if(!modelObjectsPair.first){
            loggerPtr->log(LOG_ERROR, "Models retrieving failed from material:" + material.getTag());
            return std::vector<Model *>();
        }
        std::vector<Model *> models;
        for(auto it=modelObjectsPair.second.cbegin(); it!= modelObjectsPair.second.cend(); it++){
            models.push_back((Model*)(*it));
        }
        return models;
    }

    void MaterialService::removeMaterialFromModel(Material &material, Model &model) {
        std::pair<bool, Object&>  object = objectServicePtr->getLinkedObjectByTag(material, model.getTag());
        if(object.first){
            objectServicePtr->unlinkObject(material, model);
            loggerPtr->log(LOG_WARN, "Material:" + material.getTag() + " detached from Model:"+model.getTag());
        }else{
            loggerPtr->log(LOG_INFO, "Model:" + model.getTag() + " not attached to Material:"+material.getTag()+". Hence, can't be removed.");
        }
    }

    std::pair<bool, IObjectRenderer &> MaterialService::getRenderer(Material &material) {
        return std::pair<bool, IObjectRenderer &>(true, material.getObjectRenderer());
    }

    std::pair<bool, Shader &> MaterialService::getShader(Material &material) {
        std::pair<bool, std::vector<Object *>& >  shadersPair = objectServicePtr->getLinkedObjectsByObjectType(material, "shader");
        if(!shadersPair.first){
            loggerPtr->log(LOG_ERROR, "shader retrieval failed from material:" + material.getTag());
            return std::pair<bool, Shader&> (false, *(Shader*)NULL);
        }
        //assert that shaders.size() == 1
        if(shadersPair.second.size() == 0){
            return std::pair<bool, Shader&> (false, *(Shader*)NULL);
        }else{
            return std::pair<bool, Shader &>(true, *(Shader*)shadersPair.second[0]);
        }
    }


}