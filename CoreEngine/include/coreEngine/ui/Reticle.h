#ifndef COREENGINE_RETICLE_H
#define COREENGINE_RETICLE_H

#include <coreEngine/factory/IUniformColorFactory.h>
#include <coreEngine/components/transformTree/ITransformTreeFactory.h>
#include <coreEngine/renderObjects/Camera.h>
#include <coreEngine/modelBuilder/ConcentricCircleBuilder.h>
#include <coreEngine/factory/IModelFactory.h>

namespace cl{
	class Reticle{
	public:
		Reticle(IUniformColorFactory *uniformColorFactory,
			ITransformTreeFactory *transformTreeFactory,
			TransformTree *parent,
			IModelFactory *modelFactory,
			std::string id,
			Scene *scene,
			CL_Vec4 &color);
		void setTransformTreeModel(TransformTreeModel *transformTreeModel);
		TransformTreeModel* getTransformTreeModel();
	private:
		float distanceFromCamera = 10.0f;
		unsigned int nDivisions = 20;
		float innerRadius = 0.05f;
		float outerRadius = 0.13f;

		TransformTreeModel *transformTreeModel = nullptr;
	};
}

#endif //COREENGINE_RETICLE_H