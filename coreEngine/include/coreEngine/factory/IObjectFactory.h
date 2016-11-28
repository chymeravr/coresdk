//
// Created by chimeralabs on 10/18/2016.
//

#ifndef ANDROIDSDK_IOBJECTFACTORY_H
#define ANDROIDSDK_IOBJECTFACTORY_H

#include <string>
#include <coreEngine/model/Object.h>

namespace cl{
    class IObjectFactory{
    public:
		/**
		* Factory method to create an object or its derivatives. Implementation of IObjectFactory should ask the resources it needs in the constructor.
		*/
        virtual std::unique_ptr<Object> createObject(std::string tag) = 0;
		/**
		* The operations of deinitializing resources of the object should be performed here. However, the owner of the Object must destroy the memory manually
		* or use smart_pointers
		*/
		virtual void destroyObject(Object *objectPtr) = 0;
    };
}

#endif //ANDROIDSDK_IOBJECTFACTORY_H
