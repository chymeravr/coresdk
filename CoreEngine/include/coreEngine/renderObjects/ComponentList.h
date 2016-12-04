#ifndef COREENGINE_COMPONENTLIST_H
#define COREENGINE_COMPONENTLIST_H

#include <coreEngine/renderObjects/IRelationable.h>
#include <coreEngine/renderObjects/IComponentList.h>

namespace cl{
    class ComponentList : public IRelationable, public IComponentList{
    public:
        /*
        * Implementation of IRelationable Interface
        */
        std::string getType();

        /*
        * Implementation of IComponentList Interface
        */
        IComponent *getComponent(std::string componentType);

    };
}

#endif //COREENGINE_COMPONENTLIST_H