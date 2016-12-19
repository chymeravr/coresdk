#include <glImplementation/renderObjects/CameraGL.h>

namespace cl{
    CameraGL::CameraGL(const std::string &sceneId, ILoggerFactory *loggerFactory, Scene *scene) : Camera(sceneId, loggerFactory, scene){
        logger = loggerFactory->createLogger("glImplementation::CameraGL: ");
    }
    IRenderable *CameraGL::getRenderable(){
        return this;
    }
    bool CameraGL::initialize(){
        ComponentList &componentList = getComponentList();
        std::vector<IComponent*> components = componentList.getComponents();
        for (auto it = components.cbegin(); it != components.cend(); it++){
            IRenderable *renderable = (*it)->getRenderable();
            renderable->initialize();
        }
    }
    void CameraGL::draw(){
        ComponentList &componentList = getComponentList();
        std::vector<IComponent*> components = componentList.getComponents();
        for (auto it = components.cbegin(); it != components.cend(); it++){
            IRenderable *renderable = (*it)->getRenderable();
            renderable->draw();
        }
    }
    void CameraGL::deinitialize(){
        ComponentList &componentList = getComponentList();
        std::vector<IComponent*> components = componentList.getComponents();
        for (auto it = components.cbegin(); it != components.cend(); it++){
            IRenderable *renderable = (*it)->getRenderable();
            renderable->deinitialize();
        }
    }
}