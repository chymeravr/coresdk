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
        Object(std::string objectType, std::string tag){
            this->tag = tag;
            this->objectType = objectType;
        }

         virtual const std::string &getTag()  const{
            return tag;
        }

         virtual std::string &getObjectType()  {
            return objectType;
        }

         std::unordered_map<std::string, Object *> &getAttachedObjectsByTag()  {
            return attachedObjectsByTag;
        }

        void setAttachedObjectsByTag(
                 std::unordered_map<std::string, Object *> &attachedObjectsByTag) {
            Object::attachedObjectsByTag = attachedObjectsByTag;
        }

        std::unordered_map<std::string, std::vector<Object *>> &getAttachedObjectsByObjectType()  {
            return attachedObjectsByObjectType;
        }

        std::unordered_map<std::string, IComponent* > &getComponentsByType(){
            return componentsByType;
        };

        friend bool operator == (const Object &object1, const Object &object2){
            return object1.getTag() == object2.getTag();
        }

    };
}

#endif //ANDROIDSDK_OBJECT_H
