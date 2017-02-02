#ifndef COREENGINE_ITRANSFORMTREEFACTORY_H
#define COREENGINE_ITRANSFORMTREEFACTORY_H

#include <coreEngine/components/transformTree/TransformTreeCamera.h>
#include <coreEngine/components/transformTree/TransformTreeModel.h>

namespace cl{
	class ITransformTreeFactory{
	public:
		virtual std::unique_ptr<TransformTreeCamera> createTransformTreeCamera(Camera *camera) = 0;
		virtual std::unique_ptr<TransformTreeModel> createTransformTreeModel(Model *model) = 0;
	};
}

#endif //COREENGINE_ITRANSFORMTREEFACTORY_H