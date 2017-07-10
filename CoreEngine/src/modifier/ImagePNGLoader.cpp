#include <SOIL.h>
#include <coreEngine/modifier/ImagePNGLoader.h>
#include <string>

namespace cl {
ImagePNGLoader::ImagePNGLoader(ILogger* logger) { this->logger = logger; }

std::unique_ptr<Image> ImagePNGLoader::loadImage(std::string imagePath) {
  // Data read from the header of the BMP file
  int width, height, channels;
  std::unique_ptr<unsigned char> data(SOIL_load_image(
      imagePath.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA));

  // logger->log(LOG_DEBUG, "Image Width: " + std::to_string(width));
  // logger->log(LOG_DEBUG, "Image Height: " + std::to_string(height));
  // logger->log(LOG_DEBUG, "Image Channels: " + std::to_string(channels));

  invertImage(data.get(), width, height, channels);

  std::unique_ptr<Image> image(new Image);
  image->data = std::move(data);
  image->width = width;
  image->height = height;
  image->dataSize = height * width * channels;

  return std::move(image);
}

void ImagePNGLoader::invertImage(unsigned char* data, unsigned int width,
                                 unsigned int height, unsigned int channels) {
  for (int i = 0; i < height / 2; i++) {
    for (int j = 0; j < width * channels; j++) {
      // logger->log(LOG_DEBUG, std::to_string(i) + ", " + std::to_string(j));
      unsigned int firstIndex = (i * width * channels) + j;
      unsigned int lastIndex = ((height - i - 1) * width * channels) + j;
      unsigned char temp = data[firstIndex];
      data[firstIndex] = data[lastIndex];
      data[lastIndex] = temp;
    }
  }
}
}