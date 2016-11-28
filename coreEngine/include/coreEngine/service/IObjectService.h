//
// Created by chimeralabs on 10/15/2016.
//

#ifndef ANDROIDSDK_IOBJECTSERVICE_H
#define ANDROIDSDK_IOBJECTSERVICE_H

#include <string>
#include <vector>
#include <memory>

#include <coreEngine/model/Object.h>

namespace cl{
    class IObjectService{
    public:
        virtual void linkObject(Object *objectPtr1, Object *objectPtr2) = 0;
        virtual void unlinkObject(Object *objectPtr1, Object *objectPtr2) = 0;
        virtual Object* getLinkedObjectByTag(Object *objectPtr, const std::string &tag) = 0;
        virtual std::vector<Object *> getLinkedObjectsByObjectType(Object *objectPtr, const std::string &type) = 0;
    };
}

#endif //ANDROIDSDK_IOBJECTSERVICE_H
