//
// Created by chimeralabs on 11/2/2016.
//

#include <coreEngine/service/defaultImpl/ComponentService.h>
#include <coreEngine/util/LoggerFactory.h>
#include <coreEngine/components/defaultImpl/Transform.h>

namespace cl{
    ComponentService::ComponentService() {
        loggerPtr = &LoggerFactory::getInstance().getLoggerInstance("app::coreEngine::ComponentService:");
    }

    COMPONENT_ERROR ComponentService::createComponent(ComponentStore &componentStore,
                                                      IComponentFactory &componentFactory,
                                                      Object &object) {
        std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<IComponent> > > &componentsByTypeByObjectTag = componentStore.getComponentsByTypeByObjectTag();
        std::unique_ptr<IComponent> componentPtr = componentFactory.create(object);
        if(!componentPtr){
            loggerPtr->log(LOG_ERROR, "Component creation failed in factory for object =" + object.getTag());
            return COMPONENT_FACTORY_CREATION_ERROR;
        }
        std::string componentType = componentPtr->getComponentType();
        if(isComponentTypeExists(object, componentType)){ //Checking if component exists already with object. If yes, then destroy it and proceed further
            loggerPtr->log(LOG_WARN, "Component type = " + componentType + " already exist for object = " + object.getTag() + ". Replacing it with new one");
            destroyComponent(componentStore, componentFactory, object, *componentsByTypeByObjectTag[object.getTag()][componentType].get());
        }
        //making the componentStore the owner of componentPtr
        componentsByTypeByObjectTag[object.getTag()][componentType] = std::move(componentPtr);
        IComponent &componentCopy = *componentsByTypeByObjectTag[object.getTag()][componentType].get();

        //setting reference of component in the object
        std::unordered_map<std::string, IComponent*> &objectComponents = object.getComponentsByType();
        objectComponents[componentType] = &componentCopy;

        //setting reference in componentStore for variable objectsByComponentType
        std::unordered_map<std::string, std::vector<Object*> > &objectsByComponentType = componentStore.getObjectsByComponentType();
        std::vector<Object*> &objects = objectsByComponentType[componentType];
        objects.push_back(&object);

        loggerPtr->log(LOG_INFO, "Component of type = " + componentType + " created for object = " + object.getTag());

        return COMPONENT_NO_ERROR;
    }

    bool ComponentService::isComponentTypeExists(Object &object, std::string type) {
        std::unordered_map<std::string, IComponent* > &components = object.getComponentsByType();
        if(components.find(type) == components.cend()){
            return false;
        }else{
            return true;
        }

    }

    COMPONENT_ERROR ComponentService::destroyComponent(ComponentStore &componentStore,
                                                       IComponentFactory &componentFactory,
                                                       Object &object, IComponent &component) {
        std::string componentType = component.getComponentType();
        if(isComponentTypeExists(object, componentType)){ //Checking if component exists already with object. If yes, then destroy it and proceed further
            //removing component from the object
            std::unordered_map<std::string, IComponent* > &objectComponents = object.getComponentsByType();
            objectComponents.erase(componentType);

            //removing object from the componentstore
            std::unordered_map<std::string, std::vector<Object*> > &objectsByComponentType = componentStore.getObjectsByComponentType();
            std::vector<Object*> &objects = objectsByComponentType[componentType];
            for(auto it = objects.cbegin(); it!=objects.cend(); it++){
                if((*it)->getTag() == object.getTag()){
                    objects.erase(it);
                }
            }

            // Use factory to destroy component
            componentFactory.destroy(component);

            // Remove the component unique ptr from component store
            std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<IComponent> > > &componentsByTypeByObjectTag = componentStore.getComponentsByTypeByObjectTag();
            componentsByTypeByObjectTag[object.getTag()].erase(componentType);
            loggerPtr->log(LOG_INFO, "Component of type = " + componentType + " destroyed for object = " + object.getTag());
        }else{
            loggerPtr->log(LOG_WARN, "Component of type " + componentType + " can't be destroyed as it doesn't exist in Object = " + object.getTag());
        }

        return COMPONENT_NO_ERROR;
    }

    std::pair<bool, IComponent &> ComponentService::getComponent(Object &object, std::string type) {
        std::unordered_map<std::string, IComponent* > &objectComponents = object.getComponentsByType();
        if(isComponentTypeExists(object, type)){
            return std::pair<bool, IComponent &>(true, *objectComponents[type]);
        }else{
            return std::pair<bool, IComponent &>(false, *(IComponent*)NULL);
        }
    }

    std::vector<IComponent *> ComponentService::getComponents(Object &object) {
        std::unordered_map<std::string, IComponent* > &components = object.getComponentsByType();
        std::vector<IComponent *> componentVec;
        for(auto it = components.cbegin(); it!=components.cend(); it++) {
            componentVec.push_back(((*it).second));
        }
        return componentVec;
    }

    std::vector<Object *> &ComponentService::getObjectsByComponentType(ComponentStore &componentStore,
                                                              std::string type) {
        std::unordered_map<std::string, std::vector<Object*> > &objects = componentStore.getObjectsByComponentType();
        return objects[type];
    }

}