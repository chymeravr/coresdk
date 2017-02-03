#ifndef COREENGINE_FONTSTORE_H
#define COREENGINE_FONTSTORE_H

#include <string>
#include <unordered_map>
#include <coreEngine/renderObjects/Texture.h>
#include <coreEngine/renderObjects/Scene.h>
#include <coreEngine/ui/Character.h>
#include <coreEngine/ui/ITextMaterialFactory.h>

namespace cl{
	class FontStore{
	public:
		FontStore(Scene *scene, std::string fontName, ITextMaterialFactory *textMaterialFactory, ILoggerFactory *loggerFactory);
		/*	
			Call this function each time to load a new fontSize
			font.ttf would be stored at font/fontName.ttf
		*/
		void loadFont(int fontSize);
		void unloadFont(int fontSize);
		Character *getCharacter(char character, int fontSize);
	private:
		std::unique_ptr<ILogger> logger;
		Scene *scene = nullptr;
		std::string fontName = "";
		ITextMaterialFactory *textMaterialFactory;
		std::unordered_map< std::string, std::unordered_map<int, std::unique_ptr<Character> > > characters;
	};
}

#endif //COREENGINE_FONTSTORE_H