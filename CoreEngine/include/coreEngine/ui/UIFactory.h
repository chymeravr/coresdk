#ifndef COREENGINE_UIFACTORY_H
#define COREENGINE_UIFACTORY_H

#include <coreEngine/ui/PlanarBackground.h>
namespace cl{
	class UIFactory{
	public:
		UIFactory(ILoggerFactory *loggerFactory,
				  std::unique_ptr<IModelFactory> modelFactory,
				  std::unique_ptr<IUniformColorFactory> uniformColorFactory,
				  std::unique_ptr<ITransformTreeFactory> transformTreeFactory){
			assert(loggerFactory != nullptr);
			assert(modelFactory != nullptr);
			assert(uniformColorFactory != nullptr);
			assert(transformTreeFactory != nullptr);
			this->loggerFactory = loggerFactory;
			this->modelFactory = std::move(modelFactory);
			this->uniformColorFactory = std::move(uniformColorFactory);
			this->transformTreeFactory = std::move(transformTreeFactory);
		}
		std::unique_ptr<PlanarBackground> createPlanarBackground(std::string id, 
																 Scene *scene,
																 CL_Vec4 color,
																 CL_Vec3 &localPosition,
																 CL_Vec3 &localRotation,
																 float width,
																 float height){
			return std::unique_ptr<PlanarBackground>(new PlanarBackground(id, modelFactory.get(), uniformColorFactory.get(), scene, color, transformTreeFactory.get(), localPosition, localRotation, width, height, loggerFactory));
		}
	private:
		ILoggerFactory *loggerFactory;
		std::unique_ptr<IModelFactory> modelFactory;
		std::unique_ptr<IUniformColorFactory> uniformColorFactory;
		std::unique_ptr<ITransformTreeFactory> transformTreeFactory;
	};
}

#endif //COREENGINE_UIFACTORY_H