#ifndef COREENGINE_PLANARBACKGROUND_H
#define COREENGINE_PLANARBACKGROUND_H

#include <coreEngine/conf/Types.h>
#include <coreEngine/ui/UIElement.h>
#include <coreEngine/components/transformTree/ITransformTreeFactory.h>
#include <coreEngine/renderObjects/Scene.h>
#include <coreEngine/factory/IUniformColorFactory.h>
#include <coreEngine/factory/IModelFactory.h>

namespace cl{
	class PlanarBackground : public UIElement {
	public:
		PlanarBackground(std::string id,
						 IModelFactory *modelFactory,
						 IUniformColorFactory *uniformColorFactory, 
						 Scene *scene, 
						 CL_Vec4 color, 
						 ITransformTreeFactory *transformTreeFactory,
						 CL_Vec3 &localPosition,
						 CL_Vec3 &localRotation,
						 float width,
						 float height,
						 ILoggerFactory *loggerFactory);
	protected:
		float width = 1.0f;
		float height = 1.0f;
		std::string id;
		std::string modelId;
		std::string materialId;
		std::string shaderId;
		Model *model = nullptr;
		ShaderUniformColor *shader = nullptr;
		MaterialUniformColor *material = nullptr;
	};
}

#endif //COREENGINE_PLANARBACKGROUND_H