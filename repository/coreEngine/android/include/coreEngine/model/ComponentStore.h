//
// Created by chimeralabs on 11/2/2016.
//

#ifndef ANDROIDSDK_COMPONENTSTORE_H
#define ANDROIDSDK_COMPONENTSTORE_H

#include <string>
#include <unordered_map>
#include <vector>

#include <coreEngine/model/Object.h>

namespace cl{
    class ComponentStore{
    private:
        /**
         * This is just copy of componentsByTypeByObjectTag for faster access to queries like get all components of a particular component type
         */
        std::unordered_map<std::string, std::vector<Object*> > objectsByComponentType;
        /*
         * Stores components of a particular object of a particular componentType. So, it is a map of objectTag and another map M. Where M is a map of
         * componentType and component for a particular object of objectTag. Component Management system will not worry about uniqueness of objectTag.
         */
        std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<IComponent> > > componentsByTypeByObjectTag;
    public:
        std::unordered_map<std::string, std::vector<Object*> > &getObjectsByComponentType(){
            return objectsByComponentType;
        };
        std::unordered_map<std::string, std::unordered_map<std::string, std::unique_ptr<IComponent> > > &getComponentsByTypeByObjectTag(){
            return componentsByTypeByObjectTag;
        };
    };
}

#endif //ANDROIDSDK_COMPONENTSTORE_H
