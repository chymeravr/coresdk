#include <coreEngine/renderObjects/ComponentList.h>

namespace cl{
    IComponent *ComponentList::getComponent(std::string componentType){
        if (components.find(componentType) == components.cend()){
            return nullptr;
        }
        else{
            return components[componentType].get();
        }
    }
    void ComponentList::addComponent(std::unique_ptr<IComponent> component){
        components[component->getComponentType()] = std::move(component);
    }
    void ComponentList::removeComponent(std::string componentType){
        components.erase(componentType);
    }
}