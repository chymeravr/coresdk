//
// Created by chimeralabs on 10/15/2016.
//

#ifndef ANDROIDSDK_ISCENESERVICE_H
#define ANDROIDSDK_ISCENESERVICE_H

#include <string>
#include <vector>

#include <coreEngine/model/Object.h>
#include <coreEngine/model/Scene.h>
#include <coreEngine/factory/IObjectFactory.h>
#include <coreEngine/factory/IObjectFactoryParam.h>

namespace cl{
    enum SCENE_ERROR_CODE{
        SCENE_NO_ERROR,
        SCENE_TAG_DUPLICATION,
        SCENE_FACTORY_OBJECT_CREATION_ERROR,
        SCENE_OBJECT_NOT_IN_SCENE
    };
    class ISceneService{
    public:
        virtual SCENE_ERROR_CODE createObject(Scene &scene, IObjectFactory& factory, IObjectFactoryParam &param) = 0;
        virtual SCENE_ERROR_CODE removeObject(Scene &scene, IObjectFactory& factory, Object &object) = 0;
        virtual bool isTagAvailable(Scene &scene, std::string tag) = 0;
        virtual std::pair<bool, Object&> getObjectByTag(Scene &scene, std::string tag) = 0;
        virtual std::vector<Object*>& getObjectsByObjectType(Scene &scene, std::string objectType) = 0;
        virtual std::pair<bool, IObjectRenderer&> getSceneRenderer(Scene &scene) = 0;
    };
}

#endif //ANDROIDSDK_ISCENESERVICE_H
