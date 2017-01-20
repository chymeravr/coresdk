//
// Created by robin_chimera on 1/9/2017.
//

#include "CameraGLOVR.h"
#include <coreEngine/renderObjects/IRenderable.h>

namespace cl{
    CameraGLOVR::CameraGLOVR(const std::string &sceneId, ILoggerFactory *loggerFactory,
                             Scene *scene) : Camera(sceneId, loggerFactory, scene){
        logger = loggerFactory->createLogger("image360::CameraGLOVR");
    }

    IRenderable *CameraGLOVR::getRenderable(){
        return this;
    }
    bool CameraGLOVR::initialize(){
        ComponentList &componentList = getComponentList();
        std::vector<IComponent*> components = componentList.getComponents();
        for (auto it = components.cbegin(); it != components.cend(); it++){
            IRenderable *renderable = (*it)->getRenderable();
            renderable->initialize();
        }
        logger->log(LOG_INFO, "Camera:" + sceneId + " initialized.");
        return true;
    }
    void CameraGLOVR::draw(){
        ComponentList &componentList = getComponentList();
        std::vector<IComponent*> components = componentList.getComponents();
        for (auto it = components.cbegin(); it != components.cend(); it++){
            IRenderable *renderable = (*it)->getRenderable();
            renderable->draw();
        }
    }
    void CameraGLOVR::deinitialize(){
        ComponentList &componentList = getComponentList();
        std::vector<IComponent*> components = componentList.getComponents();
        for (auto it = components.cbegin(); it != components.cend(); it++){
            IRenderable *renderable = (*it)->getRenderable();
            renderable->deinitialize();
        }
    }

    CL_Mat44 CameraGLOVR::setViewMatrix(CL_Mat44 viewMatrix){
        this->viewMatrix = viewMatrix;
    }

    CL_Mat44 CameraGLOVR::setProjectionMatrix(CL_Mat44 projectionMatrix){
        this->projectionMatrix = projectionMatrix;
    }

    void CameraGLOVR::calculateViewMatrix(){
    }

    void CameraGLOVR::calculateProjectionMatrix(){
    }
}