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
#include <coreEngine/model/ISceneRenderer.h>

namespace cl{
    class Scene{
    protected:
        std::unordered_map<std::string, std::unique_ptr<Object> > objectsByTag;
        std::unordered_map<std::string, std::vector<Object*> > objectsByObjectType;
        std::unique_ptr<ISceneRenderer> sceneRendererPtr;
    public:
        Scene(std::unique_ptr<ISceneRenderer> sceneRendererPtr){
            this->sceneRendererPtr = std::move(sceneRendererPtr);
        }

        /**
         * Returns reference to objectsByObjectType. Can be changed by the caller, possibly by a service layer. No setter for this.
         */
        std::unordered_map<std::string, std::vector<Object *>> &getObjectsByObjectType() {
            return objectsByObjectType;
        }

        /**
         * Returns reference to objectsByTag. Can be changed by the caller, possibly by a service layer. No setter for this.
         */
        std::unordered_map<std::string, std::unique_ptr<Object> > &getObjectsByTag(){
            return objectsByTag;
        };

        ISceneRenderer *getRendererPtr(){
            return sceneRendererPtr.get();
        }
    };
}

#endif //ANDROIDSDK_SCENE_H
