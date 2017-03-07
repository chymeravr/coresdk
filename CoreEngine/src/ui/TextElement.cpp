#include <coreEngine/ui/TextElement.h>
#include <coreEngine/ui/UIFactory.h>
#include <assert.h>

#include <sstream>


namespace cl{
	template <typename T>
	std::string to_string(T value)
	{
		std::ostringstream os ;
		os << value ;
		return os.str() ;
	}


	TextElement::TextElement(std::string id,
		FontStore *fontStore,
		TextStyle *textStyle,
		std::string text,
		ITransformTreeFactory *transformTreeFactory,
		CL_Vec3 &localPosition,
		CL_Vec3 &localRotation,
		UIFactory *uiFactory,
		Scene *scene){

		assert(fontStore != nullptr);
		assert(textStyle != nullptr);
		assert(uiFactory != nullptr);
		assert(scene != nullptr);

		//Creating local transform
		transformTreeUptr = transformTreeFactory->createTransformTree();
		transformTreeUptr->setLocalPosition(localPosition);
		transformTreeUptr->setLocalRotation(localRotation);
		this->setTransformTree(transformTreeUptr.get());

		CL_GLfloat x = 0.0f;
		CL_GLfloat y = 0.0f;
		for (unsigned int i = 0; i < text.size(); i++){
			char c = text[i];
			Character *character = fontStore->getCharacter(c, textStyle->fontSize);

			std::string modelId = id + "Model" + to_string(i);

			GLfloat xpos = x + character->bearing.x * textStyle->scale;
			GLfloat ypos = y - (character->size.y - character->bearing.y) * textStyle->scale;

			GLfloat w = character->size.x * textStyle->scale;
			GLfloat h = character->size.y * textStyle->scale;

			MaterialText *material = fontStore->getMaterial(textStyle, c);

			//create CharacterElement
			std::unique_ptr<CharacterElement> characterElement = uiFactory->createCharacterElement(modelId, CL_Vec2(xpos, ypos), h, w, material, scene);
			this->addChild(modelId, std::move(characterElement));
			x += (character->advance >> 6) * textStyle->scale;
		}
	}
}