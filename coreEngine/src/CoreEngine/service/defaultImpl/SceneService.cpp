//
// Created by chimeralabs on 10/26/2016.
//
#include <coreEngine/service/defaultImpl/SceneService.h>
#include <assert.h>

namespace cl{

    SceneService::SceneService(ILoggerFactory *loggerFactoryPtr) {
        assert(loggerFactoryPtr != nullptr);
        loggerPtr = std::move(loggerFactoryPtr->createLogger("app::coreEngine::SceneService:"));
    }

    Object* SceneService::createObject(Scene *scenePtr, IObjectFactory *factoryPtr, const std::string &tag) {
        assert(scenePtr != nullptr);
        assert(factoryPtr != nullptr);
        if (!isTagAvailable(scenePtr, tag)){
            loggerPtr->log(LOG_WARN, "Object can't be created. Tag duplication. Tag = " + tag);
            return nullptr;
        }
        std::unordered_map<std::string, std::unique_ptr<Object> > &objects = scenePtr->getObjectsByTag();

        objects[tag] = factoryPtr->createObject(tag);
        assert(objects[tag] != nullptr);
        std::unordered_map<std::string, std::vector<Object *>> &objectsByType = scenePtr->getObjectsByObjectType();
        Object *objectPtr = objects[tag].get();
        objectsByType[objectPtr->getObjectType()].push_back(objectPtr);
        loggerPtr->log(LOG_INFO, "Object created with tag:" + tag);
        return objectPtr;
    }

    void SceneService::removeObject(Scene *scenePtr, IObjectFactory *factoryPtr, Object *objectPtr) {
        assert(scenePtr != nullptr);
        assert(factoryPtr != nullptr);
        assert(objectPtr != nullptr);

        std::string tag = objectPtr->getTag();
        std::string type = objectPtr->getObjectType();
        if (isTagAvailable(scenePtr, tag)){
            loggerPtr->log(LOG_WARN, "Object can't be destroyed. Object is not part of scene. Tag = " + tag);
            return;
        }

        factoryPtr->destroyObject(objectPtr);

        std::unordered_map<std::string, std::vector<Object *>> &objectsByTypeMap = scenePtr->getObjectsByObjectType();
        std::vector<Object *> &objectsByType = objectsByTypeMap[type];

        auto it = objectsByType.begin();
        for (; it != objectsByType.cend(); it++){
            if ((**it) == (*objectPtr)){
                break;
            }
        }
        //auto it = std::find(objectsByType.begin(), objectsByType.end(), object);
        objectsByType.erase(it);

        std::unordered_map<std::string, std::unique_ptr<Object> > &objects = scenePtr->getObjectsByTag();
        objects.erase(tag);
        loggerPtr->log(LOG_INFO, "Object created with tag:" + tag);
        return;
    }

    bool SceneService::isTagAvailable(Scene *scenePtr, const std::string &tag) {
        assert(scenePtr != nullptr);
        std::unordered_map<std::string, std::unique_ptr<Object> > & objects = scenePtr->getObjectsByTag();
        if (objects.find(tag) == objects.cend()){
            return true;
        }
        else{
            return false;
        }
    }

    Object * SceneService::getObjectByTag(Scene *scenePtr, const std::string &tag) {
        assert(scenePtr != nullptr);
        std::unordered_map<std::string, std::unique_ptr<Object> > &objects = scenePtr->getObjectsByTag();
        if (objects.find(tag) == objects.cend()){
            return nullptr;
        }
        else{
            return objects[tag].get();
        }
    }

    std::vector<Object*> SceneService::getObjectsByObjectType(Scene *scenePtr, const std::string &objectType) {
        assert(scenePtr != nullptr);
        return scenePtr->getObjectsByObjectType()[objectType];
    }

}