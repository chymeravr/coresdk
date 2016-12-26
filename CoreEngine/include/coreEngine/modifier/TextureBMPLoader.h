#ifndef COREENGINE_TEXTUREBMPLOADER_H
#define COREENGINE_TEXTUREBMPLOADER_H

#include <coreEngine/renderObjects/Texture.h>

namespace cl{
    class TextureBMPLoader{
    public:
        TextureBMPLoader(ILogger *logger);
        bool loadImage(Texture *texture, std::string imagePath);
    private:
        ILogger* logger;
    };
}

#endif //COREENGINE_TEXTUREBMPLOADER_H