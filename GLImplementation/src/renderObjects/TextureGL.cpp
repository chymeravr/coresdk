#include <assert.h>
#include <glImplementation/renderObjects/TextureGL.h>

namespace cl{
	TextureGL::TextureGL(const std::string &sceneId, ILoggerFactory *loggerFactory) :Texture(sceneId, loggerFactory){
		logger = loggerFactory->createLogger("glImplementation::TextureGL: ");
	}
	IRenderable *TextureGL::getRenderable(){
		return this;
	}
	bool TextureGL::initialize(){
		glGenTextures(1, &textureId);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, textureId);

		// Give the image to OpenGL
		assert(data != nullptr);
		
		
		// ... nice trilinear filtering.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.get());

		glGenerateMipmap(GL_TEXTURE_2D);

		// OpenGL has now copied the data. Free our own version
		this->clearTextureData();
		logger->log(LOG_INFO, "texture:" + sceneId + " initialized");
		return true;
	}
	void TextureGL::draw(){
		glBindTexture(GL_TEXTURE_2D, textureId);
	}
	void TextureGL::deinitialize(){
		glDeleteTextures(1, &textureId);
	}
}