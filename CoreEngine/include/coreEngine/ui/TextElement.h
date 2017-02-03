#ifndef COREENGINE_TEXTELEMENT_H
#define COREENGINE_TEXTELEMENT_H

#include <coreEngine/ui/UIElement.h>
#include <coreEngine/ui/FontStore.h>
#include <coreEngine/ui/TextStyle.h>
#include <coreEngine/factory/IModelFactory.h>
#include <coreEngine/renderObjects/Scene.h>

namespace cl{
	class TextElement : UIElement{
	public:
		TextElement(std::string id, FontStore *fontStore, TextStyle *textStyle, std::string text, Scene *scene, IModelFactory *modelFactory){
			this->id = id;
			this->modelId = id + "Model";
			this->materialId = id + "Material";
			this->shaderId = id + "Shader";
			
			for (int i = 0; i < text.size(); i++){
				char c = text[i];
				Character *character = fontStore->getCharacter(c, textStyle->fontSize);

			}
		}
	private:
		std::string id;
		std::string modelId;
		std::string materialId;
		std::string shaderId;
	};
}

#endif //COREENGINE_TEXTELEMENT_H