#include <glImplementation/renderObjects/CameraGL.h>

namespace cl{
	CameraGL::CameraGL(const std::string &sceneId, ILoggerFactory *loggerFactory) : Camera(sceneId, loggerFactory){
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
		logger->log(LOG_INFO, "Camera:" + sceneId + " initialized.");
		return true;
	}
	void CameraGL::draw(){
		ComponentList &componentList = getComponentList();
		std::vector<IComponent*> components = componentList.getComponents();
		for (auto it = components.cbegin(); it != components.cend(); it++){
			IRenderable *renderable = (*it)->getRenderable();
			renderable->draw();
		}
		calculateProjectionMatrix();
		calculateViewMatrix();
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