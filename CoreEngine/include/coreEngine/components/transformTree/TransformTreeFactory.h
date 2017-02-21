#ifndef COREENGINE_TRANSFORMTREEFACTORY_H
#define COREENGINE_TRANSFORMTREEFACTORY_H

#include <coreEngine/components/transformTree/ITransformTreeFactory.h>

namespace cl{
	class TransformTreeFactory : public ITransformTreeFactory{
	public:
		TransformTreeFactory(ILoggerFactory *loggerFactory){
			assert(loggerFactory != nullptr);
			this->loggerFactory = loggerFactory;
		}
		std::unique_ptr<TransformTreeCamera> createTransformTreeCamera(Camera *camera){
			return std::unique_ptr<TransformTreeCamera>(new TransformTreeCamera(camera, loggerFactory));
		}
		std::unique_ptr<TransformTreeModel> createTransformTreeModel(Model *model){
			return std::unique_ptr<TransformTreeModel>(new TransformTreeModel(model, loggerFactory));
		}
		std::unique_ptr<TransformTree> createTransformTree(){
			return std::unique_ptr<TransformTree>(new TransformTree);
		}
	private:
		ILoggerFactory *loggerFactory;
	};
}

#endif //COREENGINE_TRANSFORMTREEFACTORY_H