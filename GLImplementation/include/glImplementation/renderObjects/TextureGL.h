#ifndef GLIMPLEMENTATION_TEXTUREGL_H
#define GLIMPLEMENTATION_TEXTUREGL_H

#include <coreEngine/renderObjects/Texture.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <conf/GLTypes.h>

namespace cl{
	class TextureGL : public Texture, public IRenderable{
	public:
		TextureGL(const std::string &sceneId, ILoggerFactory *loggerFactory);
		IRenderable *getRenderable();
		bool initialize();
		void draw();
		void deinitialize();

	private:
		std::unique_ptr<ILogger> logger;
		CL_GLuint textureId;
		void swapColorChannels(unsigned char* data, unsigned int width, unsigned int height, unsigned int dataSize);
	};
}

#endif //GLIMPLEMENTATION_TEXTUREGL_H