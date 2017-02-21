#ifndef GLIMPLEMENTATION_TEXTURETEXTGL_H
#define GLIMPLEMENTATION_TEXTURETEXTGL_H

#include <coreEngine/ui/TextureText.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <conf/GLTypes.h>

namespace cl{
	class TextureTextGL : public TextureText, IRenderable{
	public:
		TextureTextGL(const std::string &sceneId, ILoggerFactory *loggerFactory, const unsigned int &width, const unsigned int &height, std::unique_ptr<unsigned char> data);
		IRenderable *getRenderable();
		bool initialize();
		void draw();
		void deinitialize();
	private:
		CL_GLuint textureId;
		std::unique_ptr<ILogger> logger = nullptr;
	};
}

#endif //GLIMPLEMENTATION_TEXTURETEXTGL_H