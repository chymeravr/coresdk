#include <assert.h>
#include <coreEngine/modifier/ImageModifier.h>

namespace cl{
	ImageModifier::ImageModifier(ILogger *logger){
		this->logger = logger;
	}

	void ImageModifier::convertSingleCubicImageIntoSix(const Image *originalImage, Image *frontImage, Image *leftImage, Image *backImage, Image *rightImage, Image *topImage, Image *bottomImage){
		assert(originalImage != nullptr);
		assert(frontImage != nullptr);
		assert(leftImage != nullptr);
		assert(backImage != nullptr);
		assert(rightImage != nullptr);
		assert(topImage != nullptr);
		assert(bottomImage != nullptr);
		assert(originalImage->height == originalImage->width * 3 / 4);
		unsigned int pixels = originalImage->height * originalImage->width;
		unsigned int channels = originalImage->dataSize / pixels;
		unsigned int subImageSize = originalImage->width / 4;
		calculateSubImage(originalImage, frontImage, subImageSize * 1, subImageSize * 2 - 1, subImageSize * 1, subImageSize * 2 - 1, channels);
		calculateSubImage(originalImage, leftImage, subImageSize * 1, subImageSize * 2 - 1, 0, subImageSize * 1 - 1, channels);
		calculateSubImage(originalImage, backImage, subImageSize * 1, subImageSize * 2 - 1, subImageSize * 3, subImageSize * 4 - 1, channels);
		calculateSubImage(originalImage, rightImage, subImageSize * 1, subImageSize * 2 - 1, subImageSize * 2, subImageSize * 3 - 1, channels);
		calculateSubImage(originalImage, topImage, 0, subImageSize * 1 - 1, subImageSize * 1, subImageSize * 2 - 1, channels);
		calculateSubImage(originalImage, bottomImage, subImageSize * 2, subImageSize * 3 - 1, subImageSize * 1, subImageSize * 2 - 1, channels);
	}

	void ImageModifier::calculateSubImage(const Image *originalImage, Image *subImage, unsigned int rowLowerIndex, unsigned int rowHigherIndex, unsigned int columnLowerIndex, unsigned int columnHigherIndex, unsigned int channels){
		unsigned int totalWidth = originalImage->width;
		unsigned int subImageIndex = 0;
		subImage->width = columnHigherIndex - columnLowerIndex + 1;
		subImage->height = rowHigherIndex - rowLowerIndex + 1;
		subImage->dataSize = subImage->width * subImage->height * channels;
		subImage->data = std::unique_ptr<unsigned char>(new unsigned char[subImage->dataSize]);
		for (unsigned int i = rowLowerIndex; i <= rowHigherIndex; i++){
			for (unsigned int j = columnLowerIndex; j <= columnHigherIndex; j++){
				for (unsigned int k = 0; k < channels; k++){
					subImage->data.get()[subImageIndex++] = originalImage->data.get()[(i*totalWidth + j) * channels + k];
				}
			}
		}
	}
}