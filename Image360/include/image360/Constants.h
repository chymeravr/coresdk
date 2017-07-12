#ifndef IMAGE360_CONSTANTS_H
#define IMAGE360_CONSTANTS_H

namespace cl {
// todo : remove this
enum TEXTURE_MAP_MODE {
  CUBE_MAP_MODE_SIX_IMAGES,
  CUBE_MAP_MODE_SINGLE_IMAGE,
  EQUIRECTANGULAR_MAP_MODE
};
enum IMAGE_MODE { STEREO, MONO };

// TODO : migrate to C++11 enum standards
enum EYE { LEFT, RIGHT };
}

#endif  // IMAGE360_CONSTANTS_H