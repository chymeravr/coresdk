//
// Created by chimeralabs on 11/2/2016.
//

#ifndef ANDROIDSDK_COMPONENTSERVICE_H
#define ANDROIDSDK_COMPONENTSERVICE_H

#include <coreEngine/service/IComponentService.h>
#include <coreEngine/util/Logger.h>
#include <memory>

namespace cl{
    class ComponentService : public IComponentService{
    private:
        Logger *loggerPtr;
    public:
        ComponentService();
        COMPONENT_ERROR createComponent(ComponentStore &componentStore, IComponentFactory &componentFactory, Object &object);
        bool isComponentTypeExists(Object &object, std::string type);
        COMPONENT_ERROR destroyComponent(ComponentStore &componentStore, IComponentFactory &componentFactory, Object &object, IComponent &component);
        std::pair<bool, IComponent &> getComponent(Object &object, std::string type);
        std::vector<IComponent *> getComponents(Object &object);
        std::vector<Object *> &getObjectsByComponentType(ComponentStore &componentStore, std::string type);

    };
}

#endif //ANDROIDSDK_COMPONENTSERVICE_H
