//
// Created by chimeralabs on 11/2/2016.
//

#ifndef ANDROIDSDK_ICOMPONENTFACTORY_H
#define ANDROIDSDK_ICOMPONENTFACTORY_H

#include <memory>
#include <string>
#include <coreEngine/model/IComponent.h>
#include <coreEngine/model/Object.h>

namespace cl{
    class IComponentFactory{
    public:
        virtual std::unique_ptr<IComponent> create(Object &object) = 0;
        virtual void destroy(IComponent &object) = 0;
    };
}

#endif //ANDROIDSDK_ICOMPONENTFACTORY_H
