#ifndef COREENGINE_TEXTUREBMPLOADER_H
#define COREENGINE_TEXTUREBMPLOADER_H

#include <coreEngine/renderObjects/Texture.h>

namespace cl{
    class TextureBMPLoader{
        bool loadImage(Texture *texture, std::string imagePath);
    };
}

#endif //COREENGINE_TEXTUREBMPLOADER_H