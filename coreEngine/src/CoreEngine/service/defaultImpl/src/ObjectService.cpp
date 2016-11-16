//
// Created by chimeralabs on 10/26/2016.
//

#include <coreEngine/service/defaultImpl/ObjectService.h>
namespace cl{

    ObjectService::ObjectService() {
        loggerPtr = &LoggerFactory::getInstance().getLoggerInstance("app::coreEngine::ModelService:");
    }

    void ObjectService::linkObject(Object &object1, Object &object2) {
        std::unordered_map<std::string, Object*> &objectsByTags1 = object1.getAttachedObjectsByTag();
        std::unordered_map<std::string, std::vector<Object*> > &objectsByType1 = object1.getAttachedObjectsByObjectType();
        std::unordered_map<std::string, Object*> &objectsByTags2 = object2.getAttachedObjectsByTag();
        std::unordered_map<std::string, std::vector<Object*> > &objectsByType2 = object2.getAttachedObjectsByObjectType();
        
        if(objectsByTags1.find(object2.getTag()) == objectsByTags1.cend()){
            objectsByTags1[object2.getTag()] = &object2;
            objectsByType1[object2.getObjectType()].push_back(&object2);
            objectsByTags2[object1.getTag()] = &object1;
            objectsByType2[object1.getObjectType()].push_back(&object1);
            loggerPtr->log(LOG_INFO, "Objects: " + object1.getTag() + " and " + object2.getTag() + " have been linked");
        }else{ //already linked
            loggerPtr->log(LOG_WARN, "Objects: " + object1.getTag() + " and " + object2.getTag() + " cannot be linked as they are already linked.");
        }
    }

    void ObjectService::unlinkObject(Object &object1, Object &object2) {
        std::unordered_map<std::string, Object*> &objectsByTags1 = object1.getAttachedObjectsByTag();
        std::unordered_map<std::string, std::vector<Object*> > &objectsByType1 = object1.getAttachedObjectsByObjectType();
        std::unordered_map<std::string, Object*> &objectsByTags2 = object2.getAttachedObjectsByTag();
        std::unordered_map<std::string, std::vector<Object*> > &objectsByType2 = object2.getAttachedObjectsByObjectType();

        if(objectsByTags1.find(object2.getTag()) == objectsByTags1.cend()){
            loggerPtr->log(LOG_WARN, "Objects: " + object1.getTag() + " and " + object2.getTag() + " cannot be unlinked as they are aren't linked.");

        }else{ //already linked
            //deleting linked object of object1
            objectsByTags1.erase(object2.getTag());
            std::vector<Object*> &object1objects = objectsByType1[object2.getObjectType()];
            auto it1 = object1objects.begin();
            for(; it1!=object1objects.end(); it1++){
                if((**it1) == object2){
                    break;
                }
            }
            //auto it1 = std::find(object1objects.begin(), object1objects.end(), object2);
            object1objects.erase(it1);

            //deleting linked object of object2
            objectsByTags2.erase(object1.getTag());
            std::vector<Object*> &object2objects = objectsByType2[object1.getObjectType()];
            auto it2 = object1objects.begin();
            for(; it2!=object1objects.end(); it2++){
                if((**it2) == object2){
                    break;
                }
            }
            //auto it2 = std::find(object2objects.begin(), object2objects.end(), object1);
            object2objects.erase(it2);
            loggerPtr->log(LOG_INFO, "Objects: " + object1.getTag() + " and " + object2.getTag() + " have been unlinked");
        }
    }

    std::pair<bool, Object&> ObjectService::getLinkedObjectByTag(Object &object, std::string tag) {
        std::unordered_map<std::string, Object *> &objects = object.getAttachedObjectsByTag();
        if(objects.find("tag")==objects.cend()){
            return std::pair<bool, Object&>(false, *((Object*)NULL));
        }else{
            return std::pair<bool, Object&>(true, *objects[tag]);
        }
    }

    std::pair<bool, std::vector<Object *> &> ObjectService::getLinkedObjectsByObjectType(Object &object,
                                                                       std::string type) {
        std::unordered_map<std::string, std::vector<Object *>> & objects = object.getAttachedObjectsByObjectType();
        if(objects.find(type) == objects.cend()){
            std::vector<Object *>vec;
            return std::pair<bool, std::vector<Object *> &>(false, vec);
        }else{
            return std::pair<bool, std::vector<Object *> &>(true, objects[type]);
        }
    }

}