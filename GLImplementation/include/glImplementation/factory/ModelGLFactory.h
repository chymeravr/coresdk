#ifndef GLIMPLEMENTATION_MODELGLFACTORY_H
#define GLIMPLEMENTATION_MODELGLFACTORY_H

#include <coreEngine/factory/IModelFactory.h>
#include <glImplementation/renderObjects/ModelGL.h>

namespace cl{
	class ModelGLFactory : public IModelFactory{
	public:
		ModelGLFactory(ILoggerFactory *loggerFactory){
			this->loggerFactory = loggerFactory;
		}
		std::unique_ptr<Model> create(const std::string &sceneId){
			return std::unique_ptr<Model>(new ModelGL(sceneId, loggerFactory));
		}
	private:
		ILoggerFactory *loggerFactory;
	};
}

#endif //GLIMPLEMENTATION_MODELGLFACTORY_H