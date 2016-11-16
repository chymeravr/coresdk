//
// Created by chimeralabs on 10/15/2016.
//

#ifndef ANDROIDSDK_SCENE_H
#define ANDROIDSDK_SCENE_H

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

#include <coreEngine/model/Object.h>
#include "IObjectRenderer.h"

namespace cl{
    class Scene : public Object{
    protected:
        std::unordered_map<std::string, std::unique_ptr<Object> > objectsByTag;
        std::unordered_map<std::string, std::vector<Object*> > objectsByObjectType;
        IObjectRenderer *sceneRendererPtr;
    public:
        Scene(IObjectRenderer &sceneRenderer, std::string tag) : Object("scene", tag){
            this->sceneRendererPtr = &sceneRenderer;
        }
        void addObject(std::unique_ptr<Object> object){
            objectsByTag[object->getTag()] = std::move(object);
        }
        Object& getObject(std::string tag){
            return *objectsByTag[tag];
        }

        std::unordered_map<std::string, std::vector<Object *>> &getObjectsByObjectType() {
            return objectsByObjectType;
        }

        std::unordered_map<std::string, std::unique_ptr<Object> > &getObjectsByTag(){
            return objectsByTag;
        };

        IObjectRenderer &getRenderer(){
            return *sceneRendererPtr;
        }
    };
}

#endif //ANDROIDSDK_SCENE_H
