//
// Created by chimeralabs on 10/26/2016.
//

#include <coreEngine/service/defaultImpl/ObjectService.h>
#include <exception>
#include <assert.h>

namespace cl{

	ObjectService::ObjectService(ILoggerFactory *loggerFactoryPtr) {
        assert(loggerFactoryPtr != nullptr);
		loggerPtr = loggerFactoryPtr->createLogger("coreEngine::ModelService:");
    }

	void ObjectService::linkObject(Object *objectPtr1, Object *objectPtr2) {
		assert(objectPtr1 != nullptr);
		assert(objectPtr2 != nullptr);

        std::unordered_map<std::string, Object*> &objectsByTags1 = objectPtr1->getAttachedObjectsByTag();
        std::unordered_map<std::string, std::vector<Object*> > &objectsByType1 = objectPtr1->getAttachedObjectsByObjectType();
        std::unordered_map<std::string, Object*> &objectsByTags2 = objectPtr2->getAttachedObjectsByTag();
        std::unordered_map<std::string, std::vector<Object*> > &objectsByType2 = objectPtr2->getAttachedObjectsByObjectType();
        
		if (objectPtr1->getTag() == objectPtr2->getTag()){
			loggerPtr->log(LOG_WARN, "Objects: " + objectPtr1->getTag() + " and " + objectPtr2->getTag() + " are same. So, can't be linked");
		}
		else if(objectsByTags1.find(objectPtr2->getTag()) == objectsByTags1.cend()){
            objectsByTags1[objectPtr2->getTag()] = objectPtr2;
            objectsByType1[objectPtr2->getObjectType()].push_back(objectPtr2);
            objectsByTags2[objectPtr1->getTag()] = objectPtr1;
            objectsByType2[objectPtr1->getObjectType()].push_back(objectPtr1);
            loggerPtr->log(LOG_INFO, "Objects: " + objectPtr1->getTag() + " and " + objectPtr2->getTag() + " have been linked");
        }else{ //already linked
            loggerPtr->log(LOG_WARN, "Objects: " + objectPtr1->getTag() + " and " + objectPtr2->getTag() + " cannot be linked as they are already linked.");
        }
    }

	void ObjectService::unlinkObject(Object *objectPtr1, Object *objectPtr2) {
		assert(objectPtr1 != nullptr);
		assert(objectPtr2 != nullptr);

        std::unordered_map<std::string, Object*> &objectsByTags1 = objectPtr1->getAttachedObjectsByTag();
        std::unordered_map<std::string, std::vector<Object*> > &objectsByType1 = objectPtr1->getAttachedObjectsByObjectType();
        std::unordered_map<std::string, Object*> &objectsByTags2 = objectPtr2->getAttachedObjectsByTag();
        std::unordered_map<std::string, std::vector<Object*> > &objectsByType2 = objectPtr2->getAttachedObjectsByObjectType();

        if(objectsByTags1.find(objectPtr2->getTag()) == objectsByTags1.cend()){
            loggerPtr->log(LOG_WARN, "Objects: " + objectPtr1->getTag() + " and " + objectPtr2->getTag() + " cannot be unlinked as they are aren't linked.");

        }else{ //already linked
            //deleting linked object of object1
            objectsByTags1.erase(objectPtr2->getTag());
            std::vector<Object*> &object1objects = objectsByType1[objectPtr2->getObjectType()];
            auto it1 = object1objects.begin();
            for(; it1!=object1objects.end(); it1++){
                if((*it1)->getTag() == objectPtr2->getTag()){
                    break;
                }
            }
            object1objects.erase(it1);

            //deleting linked object of object2
            objectsByTags2.erase(objectPtr1->getTag());
            std::vector<Object*> &object2objects = objectsByType2[objectPtr1->getObjectType()];
            auto it2 = object2objects.begin();
            for(; it2!=object2objects.end(); it2++){
                if((*it2)->getTag() == objectPtr1->getTag()){
                    break;
                }
            }
            object2objects.erase(it2);
            loggerPtr->log(LOG_INFO, "Objects: " + objectPtr1->getTag() + " and " + objectPtr2->getTag() + " have been unlinked");
        }
    }

	Object* ObjectService::getLinkedObjectByTag(Object *objectPtr, const std::string &tag) {
		assert(objectPtr != nullptr);
        std::unordered_map<std::string, Object *> &objects = objectPtr->getAttachedObjectsByTag();
        if(objects.find(tag)==objects.cend()){
            return nullptr;
        }else{
            return objects[tag];
        }
    }

	std::vector<Object *> ObjectService::getLinkedObjectsByObjectType(Object *objectPtr, const std::string &type) {
		assert(objectPtr != nullptr);
        std::unordered_map<std::string, std::vector<Object *>> & objects = objectPtr->getAttachedObjectsByObjectType();
        if(objects.find(type) == objects.cend()){
			return std::vector<Object *>();
        }else{
            return objects[type];
        }
    }

}