#ifndef COREENGINE_COMPONENTLIST_H
#define COREENGINE_COMPONENTLIST_H

#include <coreEngine/renderObjects/Relation.h>
#include <coreEngine/renderObjects/IComponentList.h>

namespace cl{
    class ComponentList : public Relation, public IComponentList{
    public:
        /*
        * Implementation of Relation Interface
        */
        std::string getType();

        /*
        * Implementation of IComponentList Interface
        */
        IComponent *getComponent(std::string componentType);

    };
}

#endif //COREENGINE_COMPONENTLIST_H