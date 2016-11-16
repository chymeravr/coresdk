//
// Created by chimeralabs on 10/21/2016.
//

#ifndef ANDROIDSDK_SCENESERVICE_H
#define ANDROIDSDK_SCENESERVICE_H

#include <coreEngine/service/ISceneService.h>
#include <coreEngine/util/Logger.h>
#include <coreEngine/util/LoggerFactory.h>

namespace cl{
    class SceneService : public ISceneService{
    private:
        Logger *loggerPtr;
    public:
        SceneService();
        SCENE_ERROR_CODE createObject(Scene &scene, IObjectFactory& factory, IObjectFactoryParam &param);
        SCENE_ERROR_CODE removeObject(Scene &scene, IObjectFactory& factory, Object &object);
        bool isTagAvailable(Scene &scene, std::string tag);
        std::pair<bool, Object&> getObjectByTag(Scene &scene, std::string tag);
        std::vector<Object*>& getObjectsByObjectType(Scene &scene, std::string objectType);
        std::pair<bool, IObjectRenderer&> getSceneRenderer(Scene &scene);
    };
}

#endif //ANDROIDSDK_SCENESERVICE_H
