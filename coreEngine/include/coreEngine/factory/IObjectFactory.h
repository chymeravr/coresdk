//
// Created by chimeralabs on 10/18/2016.
//

#ifndef ANDROIDSDK_IOBJECTFACTORY_H
#define ANDROIDSDK_IOBJECTFACTORY_H

#include <coreEngine/model/Object.h>
#include <coreEngine/factory/IObjectFactoryParam.h>

namespace cl{
    class IObjectFactory{
    public:
        virtual std::unique_ptr<Object> createObject(IObjectFactoryParam &param) = 0;
        virtual void destroyObject(Object& object) = 0;
    };
}

#endif //ANDROIDSDK_IOBJECTFACTORY_H
