#ifndef COREENGINE_COMPONENTLIST_H
#define COREENGINE_COMPONENTLIST_H

#include <coreEngine/renderObjects/Relation.h>
#include <coreEngine/renderObjects/IComponent.h>

namespace cl{
    class ComponentList {
    public:
        IComponent *getComponent(std::string componentType);
        void addComponent(std::unique_ptr<IComponent> component);
        void removeComponent(std::string componentType);

    private:
        std::unordered_map<std::string, std::unique_ptr<IComponent> > components;
    };
}

#endif //COREENGINE_COMPONENTLIST_H