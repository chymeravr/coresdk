#include <assert.h>
#include <glImplementation/renderObjects/TextureCubeMapGL.h>

namespace cl{
    TextureCubeMapGL::TextureCubeMapGL(const std::string &sceneId, ILoggerFactory *loggerFactory) :TextureCubeMap(sceneId, loggerFactory){
        logger = loggerFactory->createLogger("glImplementation::TextureCubeMapGL: ");
    }
    IRenderable *TextureCubeMapGL::getRenderable(){
        return this;
    }
    bool TextureCubeMapGL::initialize(){
        assert(data[TEXTURE_CUBE_MAP_FACE_RIGHT] != nullptr);
        assert(data[TEXTURE_CUBE_MAP_FACE_LEFT] != nullptr);
        assert(data[TEXTURE_CUBE_MAP_FACE_TOP] != nullptr);
        assert(data[TEXTURE_CUBE_MAP_FACE_BOTTOM] != nullptr);
        assert(data[TEXTURE_CUBE_MAP_FACE_BACK] != nullptr);
        assert(data[TEXTURE_CUBE_MAP_FACE_FRONT] != nullptr);

        //rotating front, back, right and left faces by 180 degree to correct cubemap unwrapping unsusual convention
        rotateImageBy180(data[TEXTURE_CUBE_MAP_FACE_FRONT].get(), width[TEXTURE_CUBE_MAP_FACE_FRONT], height[TEXTURE_CUBE_MAP_FACE_FRONT], dataSize[TEXTURE_CUBE_MAP_FACE_FRONT]);
        rotateImageBy180(data[TEXTURE_CUBE_MAP_FACE_BACK].get(), width[TEXTURE_CUBE_MAP_FACE_BACK], height[TEXTURE_CUBE_MAP_FACE_BACK], dataSize[TEXTURE_CUBE_MAP_FACE_BACK]);
        rotateImageBy180(data[TEXTURE_CUBE_MAP_FACE_LEFT].get(), width[TEXTURE_CUBE_MAP_FACE_LEFT], height[TEXTURE_CUBE_MAP_FACE_LEFT], dataSize[TEXTURE_CUBE_MAP_FACE_LEFT]);
        rotateImageBy180(data[TEXTURE_CUBE_MAP_FACE_RIGHT].get(), width[TEXTURE_CUBE_MAP_FACE_RIGHT], height[TEXTURE_CUBE_MAP_FACE_RIGHT], dataSize[TEXTURE_CUBE_MAP_FACE_RIGHT]);

        glGenTextures(1, &textureId);
        // "Bind" the newly created texture : all future texture functions will modify this texture
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width[TEXTURE_CUBE_MAP_FACE_RIGHT], height[TEXTURE_CUBE_MAP_FACE_RIGHT], 0, GL_BGR, GL_UNSIGNED_BYTE, data[TEXTURE_CUBE_MAP_FACE_RIGHT].get());
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width[TEXTURE_CUBE_MAP_FACE_LEFT], height[TEXTURE_CUBE_MAP_FACE_LEFT], 0, GL_BGR, GL_UNSIGNED_BYTE, data[TEXTURE_CUBE_MAP_FACE_LEFT].get());
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width[TEXTURE_CUBE_MAP_FACE_TOP], height[TEXTURE_CUBE_MAP_FACE_TOP], 0, GL_BGR, GL_UNSIGNED_BYTE, data[TEXTURE_CUBE_MAP_FACE_TOP].get());
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width[TEXTURE_CUBE_MAP_FACE_BOTTOM], height[TEXTURE_CUBE_MAP_FACE_BOTTOM], 0, GL_BGR, GL_UNSIGNED_BYTE, data[TEXTURE_CUBE_MAP_FACE_BOTTOM].get());
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width[TEXTURE_CUBE_MAP_FACE_BACK], height[TEXTURE_CUBE_MAP_FACE_BACK], 0, GL_BGR, GL_UNSIGNED_BYTE, data[TEXTURE_CUBE_MAP_FACE_BACK].get());
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width[TEXTURE_CUBE_MAP_FACE_FRONT], height[TEXTURE_CUBE_MAP_FACE_FRONT], 0, GL_BGR, GL_UNSIGNED_BYTE, data[TEXTURE_CUBE_MAP_FACE_FRONT].get());

        // OpenGL has now copied the data. Free our own version
        this->clearTextureData();
        logger->log(LOG_INFO, "texture:" + sceneId + " initialized");
        return true;
    }
    void TextureCubeMapGL::draw(){
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    }
    void TextureCubeMapGL::deinitialize(){
        glDeleteTextures(1, &textureId);
    }
    void TextureCubeMapGL::rotateImageBy180(unsigned char* data, unsigned int width, unsigned int height, unsigned int dataSize){
        unsigned int pixels = width*height;
        unsigned int channels = dataSize / pixels;
        for (int i = 0; i < (unsigned int) (pixels/2); i++){
            for (int j = 0; j < channels; j++){
                unsigned int firstIndex = i*channels + j;
                unsigned int lastIndex = (pixels-1-i)*channels + j;
                unsigned int firstValue = data[firstIndex];
                unsigned int temp = data[firstIndex];
                data[firstIndex] = data[lastIndex];
                data[lastIndex] = temp;
            }
        }
    }
}