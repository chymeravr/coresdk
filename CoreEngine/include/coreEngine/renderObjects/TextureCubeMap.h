#ifndef COREENGINE_TEXTURECUBEMAP_H
#define COREENGINE_TEXTURECUBEMAP_H

#include <coreEngine/renderObjects/Texture.h>

namespace cl{
	enum TEXTURE_CUBE_MAP_FACE{
		TEXTURE_CUBE_MAP_FACE_LEFT,
		TEXTURE_CUBE_MAP_FACE_RIGHT,
		TEXTURE_CUBE_MAP_FACE_FRONT,
		TEXTURE_CUBE_MAP_FACE_BACK,
		TEXTURE_CUBE_MAP_FACE_TOP,
		TEXTURE_CUBE_MAP_FACE_BOTTOM
	};
	class TextureCubeMap : public Texture{
	public:
		TextureCubeMap(const std::string &sceneId, ILoggerFactory *loggerFactory) : Texture(sceneId, loggerFactory){
		}

		virtual void setTextureData(TEXTURE_CUBE_MAP_FACE face, std::unique_ptr<unsigned char> data){
			this->data[face] = std::move(data);
		}
		virtual const unsigned char  *getTextureData(TEXTURE_CUBE_MAP_FACE face){
			return this->data[face].get();
		}
		virtual void clearTextureData(){
			for (unsigned int i = 0; i < 6; i++){
				this->data[i].reset();
				this->dataSize[i] = 0;
			}
		}
		virtual void setTextureDataSize(TEXTURE_CUBE_MAP_FACE face, const unsigned int &dataSize){
			this->dataSize[face] = dataSize;
		}
		virtual const unsigned int &getTextureDataSize(TEXTURE_CUBE_MAP_FACE face){
			return this->dataSize[face];
		}
		unsigned int getWidth(TEXTURE_CUBE_MAP_FACE face){
			return width[face];
		}
		void setWidth(TEXTURE_CUBE_MAP_FACE face, const unsigned int &width){
			this->width[face] = width;
		}
		unsigned int getHeight(TEXTURE_CUBE_MAP_FACE face){
			return this->height[face];
		}
		void setHeight(TEXTURE_CUBE_MAP_FACE face, const unsigned int &height){
			this->height[face] = height;
		}

	protected:
		std::unique_ptr<ILogger> logger = nullptr;
		std::unique_ptr<unsigned char> data[6];
		unsigned int dataSize[6];
		unsigned int width[6];
		unsigned int height[6];
	};
}

#endif //COREENGINE_TEXTURECUBEMAP_H