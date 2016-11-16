//
// Created by chimeralabs on 10/26/2016.
//
#include <coreEngine/service/defaultImpl/SceneService.h>

namespace cl{

    SceneService::SceneService() {
        loggerPtr = &LoggerFactory::getInstance().getLoggerInstance("app::coreEngine::SceneService:");
    }

    SCENE_ERROR_CODE SceneService::createObject(Scene &scene, IObjectFactory &factory,
                                                IObjectFactoryParam &param) {
        if(!isTagAvailable(scene, param.getTag())){
            loggerPtr->log(LOG_WARN, "Object can't be created. Tag duplication. Tag = " + param.getTag());
            return SCENE_TAG_DUPLICATION;
        }
        std::unordered_map<std::string, std::unique_ptr<Object> > &objects = scene.getObjectsByTag();

        objects[param.getTag()] = factory.createObject(param);

        if(!objects[param.getTag()]){
            loggerPtr->log(LOG_ERROR, "Object creation failed in factory. Tag=" + param.getTag());
            return SCENE_FACTORY_OBJECT_CREATION_ERROR;
        }

        std::unordered_map<std::string, std::vector<Object *>> &objectsByType = scene.getObjectsByObjectType();
        Object *objectPtr = objects[param.getTag()].get();
        objectsByType[objectPtr->getObjectType()].push_back(objectPtr);
        loggerPtr->log(LOG_INFO, "Object created with tag:" + param.getTag());
        return SCENE_NO_ERROR;
    }

    SCENE_ERROR_CODE SceneService::removeObject(Scene &scene, IObjectFactory &factory,
                                                Object &object) {
        std::string tag = object.getTag();
        std::string type = object.getObjectType();
        if(isTagAvailable(scene, tag)){
            loggerPtr->log(LOG_WARN, "Object can't be destroyed. Object is not part of scene. Tag = " + tag);
            return SCENE_OBJECT_NOT_IN_SCENE;
        }

        factory.destroyObject(object);

        std::unordered_map<std::string, std::vector<Object *>> &objectsByTypeMap = scene.getObjectsByObjectType();
        std::vector<Object *> &objectsByType = objectsByTypeMap[type];

        auto it = objectsByType.begin();
        for(; it!=objectsByType.cend(); it++){
            if((**it) == object){
                break;
            }
        }
        //auto it = std::find(objectsByType.begin(), objectsByType.end(), object);
        objectsByType.erase(it);

        std::unordered_map<std::string, std::unique_ptr<Object> > &objects = scene.getObjectsByTag();
        objects.erase(tag);
        loggerPtr->log(LOG_INFO, "Object created with tag:" + tag);
        return SCENE_NO_ERROR;
    }

    bool SceneService::isTagAvailable(Scene &scene, std::string tag) {
        std::unordered_map<std::string, std::unique_ptr<Object> > & objects = scene.getObjectsByTag();
        if(objects.find(tag) == objects.cend()){
            return true;
        }else{
            return false;
        }
    }

    std::pair<bool, Object&> SceneService::getObjectByTag(Scene &scene, std::string tag) {
        std::unordered_map<std::string, std::unique_ptr<Object> > &objects = scene.getObjectsByTag();
        if(objects.find(tag) == objects.cend()){
            return std::pair<bool, Object&>(false, *((Object*)NULL));
        }else{
            return std::pair<bool, Object&>(true, *objects[tag].get());
        }
    }

    std::vector<Object *> &SceneService::getObjectsByObjectType(Scene &scene,
                                                                std::string objectType) {
        return scene.getObjectsByObjectType()[objectType];
    }

    std::pair<bool, IObjectRenderer&> SceneService::getSceneRenderer(Scene &scene) {
        return std::pair<bool, IObjectRenderer&> (true, scene.getRenderer());
    }
}