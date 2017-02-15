#ifndef COREENGINE_TEXTELEMENT_H
#define COREENGINE_TEXTELEMENT_H

#include <coreEngine/ui/UIElement.h>
#include <coreEngine/ui/FontStore.h>
#include <coreEngine/ui/TextStyle.h>
#include <coreEngine/factory/IModelFactory.h>
#include <coreEngine/renderObjects/Scene.h>
#include <coreEngine/ui/ITextMaterialFactory.h>
#include <coreEngine/ui/ShaderText.h>
#include <coreEngine/components/transformTree/ITransformTreeFactory.h>

namespace cl{
	class UIFactory;
	class TextElement : public UIElement{
	public:
		TextElement(std::string id,
			FontStore *fontStore,
			TextStyle *textStyle,
			std::string text,
			ITransformTreeFactory *transformTreeFactory,
			CL_Vec3 &localPosition,
			CL_Vec3 &localRotation,
			UIFactory *uiFactory,
			Scene *scene);
	private:
		std::unique_ptr<TransformTree> transformTreeUptr = nullptr;
	};
}

#endif //COREENGINE_TEXTELEMENT_H