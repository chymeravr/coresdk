#ifndef COREENGINE_TEXTURE_H
#define COREENGINE_TEXTURE_H

#include <memory>
#include <coreEngine/renderObjects/IScenable.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <coreEngine/util/ILoggerFactory.h>

namespace cl{
	class Texture : public IScenable{
	public:
		Texture(const std::string &sceneId, ILoggerFactory *loggerFactory){
			this->sceneId = sceneId;
		}
		virtual IRenderable *getRenderable() = 0;
		std::string getSceneId(){
			return this->sceneId;
		}
		virtual void setTextureData(std::unique_ptr<unsigned char> data){
			this->data = std::move(data);
		}
		virtual const unsigned char  *getTextureData(){
			return this->data.get();
		}
		virtual void clearTextureData(){
			this->data.reset();
			this->dataSize = 0;
		}
		virtual void setTextureDataSize(const unsigned int &dataSize){
			this->dataSize = dataSize;
		}
		virtual const unsigned int &getTextureDataSize(){
			return this->dataSize;
		}
		unsigned int getWidth(){
			return width;
		}
		void setWidth(const unsigned int &width){
			this->width = width;
		}
		unsigned int getHeight(){
			return this->height;
		}
		void setHeight(const unsigned int &height){
			this->height = height;
		}

	protected:
		std::string sceneId = "";
		std::unique_ptr<ILogger> logger = nullptr;
		std::unique_ptr<unsigned char> data;
		unsigned int dataSize;
		unsigned int width;
		unsigned int height;
	};
}

#endif //COREENGINE_TEXTURE_H