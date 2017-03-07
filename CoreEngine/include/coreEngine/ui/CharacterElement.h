#ifndef COREENGINE_CHARACTERELEMENT_H
#define COREENGINE_CHARACTERELEMENT_H

#include <coreEngine/ui/UIElement.h>
#include <coreEngine/conf/Types.h>
#include <coreEngine/ui/MaterialText.h>
#include <coreEngine/factory/IModelFactory.h>
#include <coreEngine/components/transformTree/ITransformTreeFactory.h>

namespace cl{
	class CharacterElement : public UIElement{
	public:
		CharacterElement(std::string id,
			CL_Vec2 origin,
			CL_GLfloat height,
			CL_GLfloat width,
			MaterialText *material,
			IModelFactory *modelFactory,
			Scene *scene,
			ITransformTreeFactory *transformTreeFactory);
		void deleteElement();
	private:
		std::string id;
		Scene *scene = nullptr;
		MaterialText *material = nullptr;
	};
}

#endif //COREENGINE_CHARACTERELEMENT_H