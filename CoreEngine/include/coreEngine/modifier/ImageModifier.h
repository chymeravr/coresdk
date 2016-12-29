#ifndef COREENGINE_IMAGEMODIFIER_H
#define COREENGINE_IMAGEMODIFIER_H

#include <coreEngine/modifier/Image.h>
#include <coreEngine/util/ILogger.h>

namespace cl{
    class ImageModifier{
    public:
        ImageModifier(ILogger *logger);
        void convertSingleCubicImageIntoSix(const Image *originalImage, Image *frontImage, Image *leftImage, Image *backImage, Image *rightImage, Image *topImage, Image *bottomImage);
    private:
        void calculateSubImage(const Image *originalImage, Image *subImage, unsigned int rowLowerIndex, unsigned int rowHigherIndex, unsigned int columnLowerIndex, unsigned int columnHigherIndex, unsigned int channels);
        ILogger *logger;
    };
}

#endif //COREENGINE_IMAGEMODIFIER_H