//
// Created by chimeralabs on 11/2/2016.
//

#include <assert.h>
#include <coreEngine/service/defaultImpl/ComponentService.h>

namespace cl{
    ComponentService::ComponentService(ILoggerFactory *loggerFactoryPtr) {
        assert(loggerFactoryPtr != nullptr);
        loggerPtr = loggerFactoryPtr->createLogger("app::coreEngine::ComponentService:");
    }

    IComponent* ComponentService::addComponentToComponentStore(ComponentStore *componentStorePtr, std::unique_ptr<IComponent> componentPtr, Object *objectPtr) {
        assert(componentStorePtr != nullptr);
        assert(componentPtr != nullptr);
        assert(objectPtr != nullptr);

        componentPtr->setObject(objectPtr);

        std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<IComponent> > > &componentsByTypeByObjectTag = componentStorePtr->getComponentsByTypeByObjectTag();
        
        std::string componentType = componentPtr->getComponentType();
        if(isComponentTypeExists(objectPtr, componentType)){ //Checking if component exists already with object. If yes, then destroy it and proceed further
            loggerPtr->log(LOG_WARN, "Component type = " + componentType + " already exist for object = " + objectPtr->getTag() + ". Replacing it with new one");
            removeComponentFromComponentStore(componentStorePtr, componentsByTypeByObjectTag[objectPtr->getTag()][componentType].get());
        }
        //making the componentStore the owner of componentPtr
        componentsByTypeByObjectTag[objectPtr->getTag()][componentType] = std::move(componentPtr);
        IComponent *componentPtrCopy = componentsByTypeByObjectTag[objectPtr->getTag()][componentType].get();

        //setting reference of component in the object
        std::unordered_map<std::string, IComponent*> &objectComponents = objectPtr->getComponentsByType();
        objectComponents[componentType] = componentPtrCopy;

        //setting reference in componentStore for variable objectsByComponentType
        std::unordered_map<std::string, std::vector<Object*> > &objectsByComponentType = componentStorePtr->getObjectsByComponentType();
        std::vector<Object*> &objects = objectsByComponentType[componentType];
        objects.push_back(objectPtr);

        loggerPtr->log(LOG_INFO, "Component of type = " + componentType + " created for object = " + objectPtr->getTag());

        return componentPtrCopy;
    }

    bool ComponentService::isComponentTypeExists(Object *objectPtr, const std::string &type) {
        assert(objectPtr != nullptr);
        std::unordered_map<std::string, IComponent* > &components = objectPtr->getComponentsByType();
        if(components.find(type) == components.cend()){
            return false;
        }else{
            return true;
        }

    }

    void ComponentService::removeComponentFromComponentStore(ComponentStore *componentStorePtr, IComponent *componentPtr) {
        assert(componentStorePtr != nullptr);
        assert(componentPtr != nullptr);
        Object *objectPtr = componentPtr->getObject();
        assert(objectPtr != nullptr);
        std::string componentType = componentPtr->getComponentType();
        if(isComponentTypeExists(objectPtr, componentType)){ //Checking if component exists already with object. If yes, then destroy it and proceed further
            //removing component from the object
            std::unordered_map<std::string, IComponent* > &objectComponents = objectPtr->getComponentsByType();
            objectComponents.erase(componentType);

            //removing object from the componentstore
            std::unordered_map<std::string, std::vector<Object*> > &objectsByComponentType = componentStorePtr->getObjectsByComponentType();
            std::vector<Object*> &objects = objectsByComponentType[componentType];
            for(auto it = objects.begin(); it!=objects.end(); it++){
                if((*it)->getTag() == objectPtr->getTag()){
                    objects.erase(it);
                    break;
                }
            }

            // Remove the component unique ptr from component store
            std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<IComponent> > > &componentsByTypeByObjectTag = componentStorePtr->getComponentsByTypeByObjectTag();
            componentsByTypeByObjectTag[objectPtr->getTag()].erase(componentType);
            loggerPtr->log(LOG_INFO, "Component of type = " + componentType + " destroyed for object = " + objectPtr->getTag());
        }else{
            loggerPtr->log(LOG_WARN, "Component of type " + componentType + " can't be destroyed as it doesn't exist in Object = " + objectPtr->getTag());
        }

        return;
    }

    IComponent* ComponentService::getComponent(Object *objectPtr, const std::string &type) {
        assert(objectPtr != nullptr);
        std::unordered_map<std::string, IComponent* > &objectComponents = objectPtr->getComponentsByType();
        if(isComponentTypeExists(objectPtr, type)){
            return objectComponents[type];
        }else{
            return nullptr;
        }
    }

    std::vector<IComponent *> ComponentService::getComponents(Object *objectPtr) {
        assert(objectPtr != nullptr);
        std::unordered_map<std::string, IComponent* > &components = objectPtr->getComponentsByType();
        std::vector<IComponent *> componentVec;
        for(auto it = components.cbegin(); it!=components.cend(); it++) {
            componentVec.push_back(((*it).second));
        }
        return componentVec;
    }

    std::vector<Object *> ComponentService::getObjectsByComponentType(ComponentStore *componentStorePtr, const std::string &type) {
        assert(componentStorePtr != nullptr);
        std::unordered_map<std::string, std::vector<Object*> > &objects = componentStorePtr->getObjectsByComponentType();
        return objects[type];
    }

}