//
// Created by chimeralabs on 11/10/2016.
//

#ifndef ANDROIDSDK_SCENEGLRENDERER_H
#define ANDROIDSDK_SCENEGLRENDERER_H

#include <coreEngine/model/IObjectRenderer.h>
#include <coreEngine/model/Object.h>
#include <coreEngine/service/ISceneService.h>
#include <coreEngine/util/Logger.h>
#include <glImplementation/service/IShaderGLService.h>

namespace cl{
    class SceneGLRenderer : public IObjectRenderer{
    private:
        Logger *loggerPtr;
        std::unique_ptr<ISceneService> sceneServicePtr;
        std::unique_ptr<IShaderGLService> shaderGLServicePtr;
    public:
        SceneGLRenderer();
        bool initialize(Object &object);
        bool draw(Object &object);
        bool deinitialize(Object &object);
    };
}

#endif //ANDROIDSDK_SCENEGLRENDERER_H
