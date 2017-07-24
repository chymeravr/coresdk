#include <string>
#include <coreEngine/modifier/ImageJPEGLoader.h>
#include <jpeglib.h>
#include <stdlib.h>

namespace cl{

ImageJPEGLoader::ImageJPEGLoader(ILogger *logger){
		this->logger = logger;
	}

	std::unique_ptr<Image> ImageJPEGLoader::loadImage(std::string imagePath){
		// Data read from the header of the BMP file
		int width, height, channels;
		auto newImage = read_JPEG_file(imagePath.c_str(), &width, &height, &channels);
		invertImage(newImage->data.get(), newImage->width, newImage->height, 3);
		return std::move(newImage);
	}

	void ImageJPEGLoader::invertImage(unsigned char* data, unsigned int width, unsigned int height, unsigned int channels)
	{
		for (int i = 0; i < height / 2; i++)
		{
			for (int j = 0; j < width * channels; j++)
			{
				//logger->log(LOG_DEBUG, std::to_string(i) + ", " + std::to_string(j));
				unsigned int firstIndex = (i*width*channels) + j ;
				unsigned int lastIndex =  ((height - i - 1)*width*channels) + j;

				unsigned char temp = data[firstIndex];
				data[firstIndex] = data[lastIndex];
				data[lastIndex] = temp;
				
			}
		}
	}

	std::unique_ptr<Image> ImageJPEGLoader::read_JPEG_file(const char * filename, int* width, int* height, int* channels)
	{
		FILE *file = fopen(filename, "rb");
		if (file == NULL)
		{
			return NULL;
		}

		struct jpeg_decompress_struct info; //for our jpeg info
		struct jpeg_error_mgr err; //the error handler

		info.err = jpeg_std_error(&err);
		jpeg_create_decompress(&info); //fills info structure

		jpeg_stdio_src(&info, file);
		jpeg_read_header(&info, TRUE);

		jpeg_start_decompress(&info);

		int w = info.output_width;
		int h = info.output_height;
		int numChannels = info.num_components; // 3 = RGB, 4 = RGBA
		unsigned long dataSize = w * h * numChannels;

		// read RGB(A) scanlines one at a time into jdata[]
		unsigned char *data = (unsigned char *)malloc(dataSize);
		unsigned char* rowptr;
		while (info.output_scanline < h)
		{
			rowptr = data + info.output_scanline * w * numChannels;
			jpeg_read_scanlines(&info, &rowptr, 1);
		}

		jpeg_finish_decompress(&info);

		fclose(file);

		std::unique_ptr<unsigned char> data4image(data);
		//free(data);

		std::unique_ptr<Image> image(new Image);
		image->data = std::move(data4image);
		image->width = w;
		image->height = h;
		image->dataSize = w * h * 3;

		return std::move(image);
	}
}