#include <coreEngine/renderObjects/ComponentList.h>

namespace cl{
	std::vector<IComponent*> ComponentList::getComponents(){
		std::vector<IComponent*> targetComponents;
		for (auto it = components.cbegin(); it != components.cend(); it++){
			targetComponents.push_back((*it).second.get());
		}
		return targetComponents;
	}
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