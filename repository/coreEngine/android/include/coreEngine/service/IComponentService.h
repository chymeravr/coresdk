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
    enum COMPONENT_ERROR{
        COMPONENT_NO_ERROR,
        COMPONENT_FACTORY_CREATION_ERROR
    };

    class IComponentService{
    public:
        /**
         * Creates Component using factory componentFactory and attaches it to the Object. If Object already has an component of same type, it is
         * deleted and replaced by the new component.
         * @args componentStore:
         * @args componentFactory: An implementation of IComponentFactory
         * @args object: To which the component is to be attached to
         */
        virtual COMPONENT_ERROR createComponent(ComponentStore &componentStore, IComponentFactory &componentFactory, Object &object) = 0;
        virtual bool isComponentTypeExists(Object &object, std::string type) = 0;
        virtual COMPONENT_ERROR destroyComponent(ComponentStore &componentStore, IComponentFactory &componentFactory, Object &object, IComponent &component) = 0;
        virtual std::pair<bool, IComponent &> getComponent(Object &object, std::string type) = 0;
        virtual std::vector<IComponent*> getComponents(Object &object) = 0;
        virtual std::vector<Object*> &getObjectsByComponentType(ComponentStore &componentStore, std::string type) = 0;
    };
}

#endif //ANDROIDSDK_ICOMPONENTSERVICE_H
