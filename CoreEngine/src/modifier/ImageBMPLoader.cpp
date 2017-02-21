#include <string>
#include <coreEngine/modifier/ImageBMPLoader.h>

namespace cl{
	ImageBMPLoader::ImageBMPLoader(ILogger *logger){
		this->logger = logger;
	}

	std::unique_ptr<Image> ImageBMPLoader::loadImage(std::string imagePath){
		// Data read from the header of the BMP file
		unsigned char header[54];
		unsigned int dataPos;
		unsigned int imageSize;
		unsigned int width, height;
		// Actual RGB data
		std::unique_ptr<unsigned char> data;

		// Open the file
		FILE * file = fopen(imagePath.c_str(), "rb");
		if (!file){
			logger->log(LOG_ERROR, imagePath + " could not be opened. Are you in the right directory ?");
			getchar();
			return nullptr;
		}

		// Read the header, i.e. the 54 first bytes

		// If less than 54 bytes are read, problem
		if (fread(header, 1, 54, file) != 54){
			logger->log(LOG_ERROR, imagePath + ": Incorrect format");
			return nullptr;
		}
		// A BMP files always begins with "BM"
		if (header[0] != 'B' || header[1] != 'M'){
			logger->log(LOG_ERROR, imagePath + ": Not correct BMP file");
			return nullptr;
		}
		// Make sure this is a 24bpp file
		if (*(int*)&(header[0x1E]) != 0)         {
			logger->log(LOG_ERROR, imagePath + ": Not correct BMP file");
			return nullptr;
		}
		if (*(int*)&(header[0x1C]) != 24)         {
			logger->log(LOG_ERROR, imagePath + ": Not correct BMP file");
			return nullptr;
		}

		// Read the information about the image
		dataPos = *(int*)&(header[0x0A]);
		imageSize = *(int*)&(header[0x22]);
		width = *(int*)&(header[0x12]);
		height = *(int*)&(header[0x16]);

		// Some BMP files are misformatted, guess missing information
		if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
		if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

		// Create a buffer
		data = std::unique_ptr<unsigned char>(new unsigned char[imageSize]);

		// Read the actual data from the file into the buffer
		fread(data.get(), 1, imageSize, file);

		// a bitmap is stored as BGR -> this is not supported by gles - lets correct for it
		swapColorChannels(data.get(), width, height, imageSize);

		// Everything is in memory now, the file wan be closed
		fclose(file);
		std::unique_ptr<Image> image(new Image);
		image->data = std::move(data);
		image->width = width;
		image->height = height;
		image->dataSize = imageSize;
		return std::move(image);
	}

	void ImageBMPLoader::swapColorChannels(unsigned char* data, unsigned int width, unsigned int height, unsigned int dataSize)
	{
		unsigned int pixels = width*height;
		unsigned int channels = dataSize / pixels;
		for (int i = 0; i < pixels - 2; i++)
		{
			unsigned int firstIndex = i*channels + 0;
			unsigned int lastIndex = i*channels + 2;
			unsigned char temp = data[firstIndex];
			data[firstIndex] = data[lastIndex];
			data[lastIndex] = temp;
		}
	}
}