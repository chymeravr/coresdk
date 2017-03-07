#ifndef COREENGINE_TEXTUREJPEGLOADER_H
#define COREENGINE_TEXTUREJPEGLOADER_H

#include <coreEngine/modifier/Image.h>
#include <coreEngine/util/ILogger.h>

namespace cl{
	class ImageJPEGLoader{
	public:
		ImageJPEGLoader(ILogger *logger);
		std::unique_ptr<Image> loadImage(std::string imagePath);
	private:
		ILogger* logger;
		void invertImage(unsigned char* data, unsigned int width, unsigned int height, unsigned int channels);
	};
}

#endif //COREENGINE_TEXTUREJPEGLOADER_H