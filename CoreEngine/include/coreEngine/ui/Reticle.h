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
	private:
		float distanceFromCamera = 0.01f;
		unsigned int nDivisions = 20;
		float innerRadius = 0.0001f;
		float outerRadius = 0.00013f;
	};
}

#endif //COREENGINE_RETICLE_H