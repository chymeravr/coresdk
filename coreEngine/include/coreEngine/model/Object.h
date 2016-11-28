//
// Created by chimeralabs on 10/15/2016.
//

#ifndef ANDROIDSDK_OBJECT_H
#define ANDROIDSDK_OBJECT_H

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

namespace cl{
    class IComponent;
    class Object{
    protected:
        std::string tag;
        std::string objectType;
        std::unordered_map<std::string, Object*> attachedObjectsByTag;
        std::unordered_map<std::string, std::vector<Object*> > attachedObjectsByObjectType;
        std::unordered_map<std::string, IComponent* > componentsByType;
    public:
        //Constructors & destructors
        Object(const std::string &objectType, const std::string &tag){
            this->tag = tag;
            this->objectType = objectType;
        }

        /**
         * Setting is done only in constructor and hence no setter for it.
         */
         virtual const std::string &getTag()  const{
            return tag;
        }

        /**
         * Setting is done only in constructor and hence no setter for it.
         */
         virtual const std::string &getObjectType()  const{
            return objectType;
        }

        /**
         * Returns reference to attachedObjectsByTag. Ideally to be called by a service layer only. It can be changed by the caller. No setter for it.
         */
         std::unordered_map<std::string, Object *> &getAttachedObjectsByTag()  {
            return attachedObjectsByTag;
        }

        /**
         * Returns reference to attachedObjectsByObjectType. Ideally to be called by a service layer only. It can be changed by the caller. No setter for it.
         */
        std::unordered_map<std::string, std::vector<Object *>> &getAttachedObjectsByObjectType()  {
            return attachedObjectsByObjectType;
        }

        /**
         * Returns reference to componentsByType. Ideally to be called by a service layer only. It can be changed by the caller. No setter for it.
         */
        std::unordered_map<std::string, IComponent* > &getComponentsByType(){
            return componentsByType;
        };

        /**
         * Returns true if tag of two objects are same else returns false.
         */
        friend bool operator == (const Object &object1, const Object &object2){
            return object1.getTag() == object2.getTag();
        }

    };
}

#endif //ANDROIDSDK_OBJECT_H
