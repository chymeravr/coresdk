#ifndef COREENGINE_IMAGE_H
#define COREENGINE_IMAGE_H

#include <memory>

namespace cl {
class Image {
 public:
  unsigned int width;
  unsigned int height;
  unsigned int dataSize;
  std::unique_ptr<unsigned char> data;
};
}

#endif  // COREENGINE_IMAGE_H