//
// Created by chimeralabs on 10/15/2016.
//

#ifndef ANDROIDSDK_ICOMPONENT_H
#define ANDROIDSDK_ICOMPONENT_H

#include <string>
#include <coreEngine/model/Object.h>

namespace cl{
    class IComponent{
    public:
        virtual std::string getComponentType() = 0;
        virtual void initialize() = 0;
        virtual void draw() = 0;
        virtual void deinitialize() = 0;
        virtual void setObject(Object &object) = 0;
        virtual Object &getObject() = 0;
    };
}

#endif //ANDROIDSDK_ICOMPONENT_H
