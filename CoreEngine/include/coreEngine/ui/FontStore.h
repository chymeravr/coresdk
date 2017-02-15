#ifndef COREENGINE_FONTSTORE_H
#define COREENGINE_FONTSTORE_H

#include <string>
#include <unordered_map>
#include <coreEngine/renderObjects/Texture.h>
#include <coreEngine/renderObjects/Scene.h>
#include <coreEngine/ui/Character.h>
#include <coreEngine/ui/ITextMaterialFactory.h>
#include <coreEngine/ui/TextStyle.h>

namespace cl{
	class FontStore{
	public:
		FontStore(Scene *scene, std::string fontName, ITextMaterialFactory *textMaterialFactory, ShaderText *shaderText, ILoggerFactory *loggerFactory);
		/*	
			Call this function each time to load a new fontSize
			font.ttf would be stored at font/fontName.ttf
		*/
		void loadFont(int fontSize);
		void unloadFont(int fontSize);
		Character *getCharacter(char character, int fontSize);
		MaterialText *getMaterial(TextStyle *textStyle, char c);
		std::string getFontName();
	private:
		std::string getTextureId(int fontSize, char c);
		std::string getMaterialId(CL_Vec4 color, int fontSize, char c);
	private:
		std::unique_ptr<ILogger> logger;
		Scene *scene = nullptr;
		std::string fontName = "";
		ITextMaterialFactory *textMaterialFactory;
		ShaderText *shaderText = nullptr;
		std::unordered_map< int, std::unordered_map<char, std::unique_ptr<Character> > > characters;
	};
}

#endif //COREENGINE_FONTSTORE_H