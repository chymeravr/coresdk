//
// Created by chimeralabs on 10/15/2016.
//

#ifndef ANDROIDSDK_ICOMPONENTSERVICE_H
#define ANDROIDSDK_ICOMPONENTSERVICE_H

#include <memory>
#include <coreEngine/model/IComponent.h>
#include <coreEngine/model/Object.h>
#include <coreEngine/factory/IComponentFactory.h>
#include <coreEngine/model/ComponentStore.h>

namespace cl{

    class IComponentService{
    public:
        /**
         * Creates Component using factory componentFactory and attaches it to the Object. If Object already has an component of same type, it is
         * deleted and replaced by the new component.
         * @args componentStorePtr:
         * @args componentPtr: An implementation of IComponent
         */
        virtual IComponent* addComponentToComponentStore(ComponentStore *componentStorePtr, std::unique_ptr<IComponent> componentPtr, Object *objectPtr) = 0;
        virtual bool isComponentTypeExists(Object *objectPtr, const std::string &type) = 0;
        virtual void removeComponentFromComponentStore(ComponentStore *componentStorePtr, IComponent *componentPtr) = 0;
        virtual IComponent* getComponent(Object *objectPtr, const std::string &type) = 0;
        virtual std::vector<IComponent*> getComponents(Object *objectPtr) = 0;
        virtual std::vector<Object*> getObjectsByComponentType(ComponentStore *componentStorePtr, const std::string &type) = 0;
    };
}

#endif //ANDROIDSDK_ICOMPONENTSERVICE_H
