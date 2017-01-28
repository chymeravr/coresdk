#ifndef COREENGINE_TEXTUREPNGLOADER_H
#define COREENGINE_TEXTUREPNGLOADER_H

#include <coreEngine/modifier/Image.h>
#include <coreEngine/util/ILogger.h>

namespace cl{
	class ImagePNGLoader{
	public:
		ImagePNGLoader(ILogger *logger);
		std::unique_ptr<Image> loadImage(std::string imagePath);
	private:
		ILogger* logger;
		void invertImage(unsigned char* data, unsigned int width, unsigned int height, unsigned int channels);
	};
}

#endif //COREENGINE_TEXTUREPNGLOADER_H