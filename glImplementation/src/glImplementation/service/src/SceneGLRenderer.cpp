//
// Created by chimeralabs on 11/10/2016.
//

#include <glImplementation/service/SceneGLRenderer.h>
#include <coreEngine/model/Scene.h>
#include <glImplementation/service/IServiceGLPool.h>
#include <coreEngine/util/LoggerFactory.h>

namespace cl{

    SceneGLRenderer::SceneGLRenderer() {
        loggerPtr = &LoggerFactory::getInstance().getLoggerInstance("app::glImplementation::SceneGLRenderer:");
        sceneServicePtr = IServiceGLPool::getInstance().getISceneService();
        shaderGLServicePtr = IServiceGLPool::getInstance().getIShaderGLService();
    }

    bool SceneGLRenderer::initialize(Object &object) {
        Scene &scene = (Scene&) object;
        std::vector<Object*>& shaders = sceneServicePtr->getObjectsByObjectType(scene, "shader");
        std::vector<Object*>& cameras = sceneServicePtr->getObjectsByObjectType(scene, "camera");
        if(cameras.size()!=1){
            loggerPtr->log(LOG_ERROR, "Camera size is not 1. Exiting");
            return false;
        }
        for(auto it=shaders.cbegin(); it!=shaders.cend(); it++){
            ShaderGL &shaderGL = (ShaderGL&)**it;
            shaderGLServicePtr->setCamera(shaderGL, (Camera&)*cameras[0]);
        }
    }

    bool SceneGLRenderer::draw(Object &object) {
        return true;
    }

    bool SceneGLRenderer::deinitialize(Object &object) {
        return true;
    }

}