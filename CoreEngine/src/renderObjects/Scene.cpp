#include <coreEngine/renderObjects/Scene.h>
#include <assert.h>

namespace cl{
    Scene::Scene(ILoggerFactory *loggerFactory, std::string id) : Relation(loggerFactory){
        assert(loggerFactory != nullptr);
        this->id = id;
        this->logger = loggerFactory->createLogger("coreEngine::Scene: ");
        logger->log(LOG_INFO, "Scene Created.");
    }
    std::string Scene::getType(){
        return this->type;
    }
    std::string Scene::getUniqueIdentifier(){
        return this->id;
    }
    void Scene::addToScene(std::unique_ptr<IScenable> scenable){
        assert(scenable != nullptr);
        assert(!exists(scenable->getSceneId()));
        scenableMap[scenable->getSceneId()] = std::move(scenable);
    }
    IScenable* Scene::getFromScene(std::string sceneId){
        if (exists(sceneId)){
            return scenableMap[sceneId].get();
        }
        else{
            return nullptr;
        }
    }
    void Scene::removeFromScene(IScenable *scenable){
        assert(scenable != nullptr);
        assert(exists(scenable->getSceneId()));
        scenableMap.erase(scenable->getSceneId());
    }
    bool Scene::exists(std::string sceneId){
        return (scenableMap.find(sceneId) != scenableMap.cend());
    }
}