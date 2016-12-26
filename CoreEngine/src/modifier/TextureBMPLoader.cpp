#include <string>
#include <coreEngine/modifier/TextureBMPLoader.h>

namespace cl{
    TextureBMPLoader::TextureBMPLoader(ILogger *logger){
        this->logger = logger;
    }

    bool TextureBMPLoader::loadImage(Texture *texture, std::string imagePath){
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
            return 0; 
        }

        // Read the header, i.e. the 54 first bytes

        // If less than 54 bytes are read, problem
        if (fread(header, 1, 54, file) != 54){
            logger->log(LOG_ERROR, imagePath + ": Incorrect format");
            return false;
        }
        // A BMP files always begins with "BM"
        if (header[0] != 'B' || header[1] != 'M'){
            logger->log(LOG_ERROR, imagePath + ": Not correct BMP file");
            return false;
        }
        // Make sure this is a 24bpp file
        if (*(int*)&(header[0x1E]) != 0)         { 
            logger->log(LOG_ERROR, imagePath + ": Not correct BMP file");
            return false; 
        }
        if (*(int*)&(header[0x1C]) != 24)         { 
            logger->log(LOG_ERROR, imagePath + ": Not correct BMP file");
            return false; 
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

        // Everything is in memory now, the file wan be closed
        fclose(file);
        texture->setTextureData(std::move(data));
        texture->setTextureDataSize(imageSize);
        texture->setWidth(width);
        texture->setHeight(height);
        return true;
    }
}