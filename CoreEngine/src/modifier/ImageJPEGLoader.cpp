#include <string>
#include <coreEngine/modifier/ImageJPEGLoader.h>
#include <SOIL.h>
#include <coreEngine/modifier/textureLoader.h>
#include <jpeglib.h>

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

namespace cl{

ImageJPEGLoader::ImageJPEGLoader(ILogger *logger){
		this->logger = logger;
	}

    // todo : use libjpeg for best performance - current one is too bad for pixel
	std::unique_ptr<Image> ImageJPEGLoader::loadImage(std::string imagePath){
		// Data read from the header of the BMP file
		int width, height, channels;
		//std::unique_ptr<unsigned char> data(SOIL_load_image(imagePath.c_str(), &width, &height, &channels, SOIL_LOAD_RGB));
		
		///std::unique_ptr<unsigned char> data(cvr_load_image(imagePath.c_str(), &width, &height, &channels, CVR_LOAD_RGB));

		//invertImage(data.get(), width, height, channels);
		
		//int t = read_JPEG_file(imagePath.c_str());
		//unsigned char* test = read_JPEG_file(imagePath.c_str(), &width, &height, &channels);
		//std::unique_ptr<unsigned char> data2(read_JPEG_file(imagePath.c_str(), &width, &height, &channels));
		auto newImage = read_JPEG_file(imagePath.c_str(), &width, &height, &channels);
		invertImage(newImage.get()->data.get(), newImage.get()->width, newImage.get()->height, 3);
	/*	std::unique_ptr<Image> image(new Image);
		image->data = std::move(data);
		image->width = width;
		image->height = height;
		image->dataSize = height * width * 3;
		*/
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

//	struct my_error_mgr {
//		struct jpeg_error_mgr pub;	/* "public" fields */
//
//		jmp_buf setjmp_buffer;	/* for return to caller */
//	};
//
//	typedef struct my_error_mgr * my_error_ptr;
//
//	/*
//	* Here's the routine that will replace the standard error_exit method:
//	*/
//
//	METHODDEF(void)
//		my_error_exit(j_common_ptr cinfo)
//	{
//		/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
//		my_error_ptr myerr = (my_error_ptr)cinfo->err;
//
//		/* Always display the message. */
//		/* We could postpone this until after returning, if we chose. */
//		(*cinfo->err->output_message) (cinfo);
//
//		/* Return control to the setjmp point */
//		longjmp(myerr->setjmp_buffer, 1);
//	}

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