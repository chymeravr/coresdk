#include <coreEngine/ui/FontStore.h>
#include <assert.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace cl{
	FontStore::FontStore(Scene *scene, std::string fontName, ITextMaterialFactory *textMaterialFactory, ILoggerFactory *loggerFactory){
		assert(scene != nullptr);
		assert(textMaterialFactory != nullptr);
		assert(loggerFactory != nullptr);
		this->scene = scene;
		this->fontName = fontName;
		this->textMaterialFactory = textMaterialFactory;
		loggerFactory->createLogger("coreEngine::FontStore: ");
	}

	void FontStore::loadFont(int fontSize){
		FT_Library ft;
		// All functions return a value different than 0 whenever an error occurred
		if (FT_Init_FreeType(&ft))
			logger->log(LOG_ERROR, "ERROR::FREETYPE: Could not init FreeType Library");

		// Load font as face
		FT_Face face;
		if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
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
			/*
			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
				);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			*/

			std::unique_ptr<Texture> textureUptr = textMaterialFactory->createTexture(fontName+"_"+std::to_string(fontSize)+"_"+std::string(1, c));
			Texture *texture = textureUptr.get();
			scene->addToScene(std::move(textureUptr));
			
			texture->setHeight(face->glyph->bitmap.rows);
			texture->setWidth(face->glyph->bitmap.width);
			std::unique_ptr<unsigned char> data(face->glyph->bitmap.buffer);
			texture->setTextureData(std::move(data));
			//TODO create new texture for text with one channel usage only GL_RED
			texture->setTextureDataSize(texture->getHeight() * texture->getWidth() * 4 * 4); //(height * width * channels * bytes/type)

			// Now store character for later use
			std::unique_ptr<Character> character(
				new Character(texture,
					glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					face->glyph->advance.x));
			
			characters[fontName][fontSize] = std::move(character);
		}
		// Destroy FreeType once we're finished
		FT_Done_Face(face);
		FT_Done_FreeType(ft);

	}

	void FontStore::unloadFont(int fontSize){

	}

	Character *getCharacter(char character, int fontSize){

	}
}