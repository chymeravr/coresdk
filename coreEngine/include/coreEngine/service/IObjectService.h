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
        virtual void linkObject(Object &object1, Object &object2) = 0;
        virtual void unlinkObject(Object &object1, Object &object2) = 0;
        virtual std::pair<bool, Object&> getLinkedObjectByTag(Object &object, std::string tag) = 0;
        virtual std::pair<bool, std::vector<Object *>& > getLinkedObjectsByObjectType(Object &object, std::string type) = 0;
    };
}

#endif //ANDROIDSDK_IOBJECTSERVICE_H
