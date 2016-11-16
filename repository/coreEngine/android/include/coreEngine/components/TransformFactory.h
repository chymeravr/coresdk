//
// Created by chimeralabs on 11/2/2016.
//

#ifndef ANDROIDSDK_TRANSFORMFACTORY_H
#define ANDROIDSDK_TRANSFORMFACTORY_H

#include <coreEngine/factory/IComponentFactory.h>
#include <coreEngine/components/defaultImpl/Transform.h>

namespace cl{
    class TransformFactory : public IComponentFactory{
        std::unique_ptr<IComponent> create(Object &object){
            std::unique_ptr<IComponent> componentPtr(new Transform);
            componentPtr->setObject(object);
            return std::move(componentPtr);
        }
        void destroy(IComponent &object){
            return;
        }
    };
}

#endif //ANDROIDSDK_TRANSFORMFACTORY_H
