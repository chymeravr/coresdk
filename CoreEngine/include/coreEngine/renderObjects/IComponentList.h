#ifndef COREENGINE_ICOMPONENTLIST_H
#define COREENGINE_COMPONENTLIST_H

#include <string>
#include <coreEngine/renderObjects/IComponent.h>

namespace cl{
    class IComponentList {
    public:
        virtual IComponent *getComponent(std::string componentType) = 0;
    };
}

#endif //COREENGINE_COMPONENTLIST_H