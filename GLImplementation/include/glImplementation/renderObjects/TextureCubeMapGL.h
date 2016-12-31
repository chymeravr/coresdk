#ifndef GLIMPLEMENTATION_TEXTURECUBEMAPGL_H
#define GLIMPLEMENTATION_TEXTURECUBEMAPGL_H

#include <coreEngine/renderObjects/TextureCubeMap.h>
#include <conf/GLTypes.h>

namespace cl{
	class TextureCubeMapGL : public TextureCubeMap, public IRenderable{
	public:
		TextureCubeMapGL(const std::string &sceneId, ILoggerFactory *loggerFactory);
		IRenderable *getRenderable();
		bool initialize();
		void draw();
		void deinitialize();

	private:
		void rotateImageBy180(unsigned char* data, unsigned int width, unsigned int height, unsigned int dataSize);
		void swapColorChannels(unsigned char* data, unsigned int width, unsigned int height, unsigned int dataSize);
		std::unique_ptr<ILogger> logger;
		CL_GLuint textureId;
	};
}

#endif //GLIMPLEMENTATION_TEXTURECUBEMAPGL_H