#include <coreEngine/ui/FontStore.h>
#include <assert.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace cl{
	FontStore::FontStore(Scene *scene, std::string fontName, ITextMaterialFactory *textMaterialFactory, ShaderText *shaderText, ILoggerFactory *loggerFactory){
		assert(scene != nullptr);
		assert(textMaterialFactory != nullptr);
		assert(shaderText != nullptr);
		assert(loggerFactory != nullptr);
		this->scene = scene;
		this->fontName = fontName;
		this->shaderText = shaderText;
		this->textMaterialFactory = textMaterialFactory;
		logger = loggerFactory->createLogger("coreEngine::FontStore: ");
	}

	void FontStore::loadFont(int fontSize){
		FT_Library ft;
		// All functions return a value different than 0 whenever an error occurred
		if (FT_Init_FreeType(&ft))
			logger->log(LOG_ERROR, "ERROR::FREETYPE: Could not init FreeType Library");

		// Load font as face
		FT_Face face;
		if (FT_New_Face(ft, fontName.c_str(), 0, &face))
			logger->log(LOG_ERROR, "ERROR::FREETYPE: Failed to load font");

		// Set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, fontSize);

		// Load first 128 characters of ASCII set
		for (GLubyte c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				logger->log(LOG_ERROR, "ERROR::FREETYTPE: Failed to load Glyph");
				continue;
			}
			std::string textureId = getTextureId(fontSize, c);

			//std::unique_ptr<unsigned char> data(face->glyph->bitmap.buffer);
			unsigned int arraySize = face->glyph->bitmap.width * face->glyph->bitmap.rows;
			std::unique_ptr<unsigned char> data(new unsigned char[arraySize]);
			for (unsigned int i = 0; i < arraySize; i++){
				data.get()[i] = face->glyph->bitmap.buffer[i];
			}
			
			std::unique_ptr<TextureText> textureUptr = textMaterialFactory->createTexture(textureId, 
																						  face->glyph->bitmap.width,
																						  face->glyph->bitmap.rows,
																						  std::move(data));
			textureUptr->setUnpackAlignment(1);
			TextureText *texture = textureUptr.get();
			scene->addToScene(std::move(textureUptr));
			
			// Now store character for later use
			std::unique_ptr<Character> character(
				new Character(glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					face->glyph->advance.x));
			
			characters[fontSize][c] = std::move(character);
		}
		// Destroy FreeType once we're finished
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

	void FontStore::unloadFont(int fontSize){

	}

	Character *FontStore::getCharacter(char character, int fontSize){
		if (characters.find(fontSize) == characters.cend()){
			loadFont(fontSize);
		}
		return characters[fontSize][character].get();
	}

	MaterialText *FontStore::getMaterial(TextStyle *textStyle, char c){
		MaterialText *material = nullptr;
		std::string materialId = getMaterialId(textStyle->color, textStyle->fontSize, c);
		if (!scene->exists(materialId)){
			TextureText *textureText = (TextureText*)scene->getFromScene(getTextureId(textStyle->fontSize, c));
			std::unique_ptr<MaterialText> materialText = textMaterialFactory->createMaterial(materialId, shaderText, textureText);
			materialText->setTextColor(textStyle->color);
			assert(materialText != nullptr);
			material = materialText.get();
			scene->addToScene(std::move(materialText));
		}
		else{
			material = (MaterialText*)scene->getFromScene(materialId);
		}
		return material;
	}

	std::string FontStore::getFontName(){
		return this->fontName;
	}

	std::string FontStore::getTextureId(int fontSize, char c){
		return fontName + "_" + std::to_string(fontSize) + "_" + std::string(1, c);
	}

	std::string FontStore::getMaterialId(CL_Vec4 color, int fontSize, char c){
		std::string colorString = std::to_string(color[0]) + "_" + std::to_string(color[1]) + "_" + std::to_string(color[2]) + "_" + std::to_string(color[3]);
		std::string materialId = this->getFontName() + "_" + std::to_string(fontSize) + "_" + colorString + "_" + std::string(1, c);
		return materialId;
	}
}