//
// Created by chimeralabs on 11/2/2016.
//

#ifndef ANDROIDSDK_COMPONENTSERVICE_H
#define ANDROIDSDK_COMPONENTSERVICE_H

#include <coreEngine/service/IComponentService.h>
#include <coreEngine/util/ILogger.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <memory>

namespace cl{
    class ComponentService : public IComponentService{
    private:
        std::unique_ptr<ILogger> loggerPtr;
    public:
        ComponentService(ILoggerFactory *loggerFactoryPtr);
        IComponent* addComponentToComponentStore(ComponentStore *componentStorePtr, std::unique_ptr<IComponent> componentPtr, Object *objectPtr);
        bool isComponentTypeExists(Object *objectPtr, const std::string &type);
        void removeComponentFromComponentStore(ComponentStore *componentStorePtr, IComponent *componentPtr);
        IComponent* getComponent(Object *objectPtr, const std::string &type);
        std::vector<IComponent*> getComponents(Object *objectPtr);
        std::vector<Object*> getObjectsByComponentType(ComponentStore *componentStorePtr, const std::string &type);
    };
}

#endif //ANDROIDSDK_COMPONENTSERVICE_H
