#ifndef COREENGINE_TEXTUREBMPLOADER_H
#define COREENGINE_TEXTUREBMPLOADER_H

#include <coreEngine/modifier/Image.h>
#include <coreEngine/util/ILogger.h>

namespace cl{
	class ImageBMPLoader{
	public:
		ImageBMPLoader(ILogger *logger);
		std::unique_ptr<Image> loadImage(std::string imagePath);
	private:
		ILogger* logger;
		void swapColorChannels(unsigned char* data, unsigned int width, unsigned int height, unsigned int dataSize);
	};
}

#endif //COREENGINE_TEXTUREBMPLOADER_H