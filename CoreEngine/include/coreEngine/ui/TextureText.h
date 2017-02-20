#ifndef COREENGINE_TEXTURETEXT_H
#define COREENGINE_TEXTURETEXT_H

#include <coreEngine/renderObjects/IScenable.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <coreEngine/renderObjects/IRenderable.h>
#include <assert.h>
#include <coreEngine/conf/Types.h>

namespace cl{
	class TextureText : public IScenable{
	public:
		TextureText(const std::string &sceneId, 
					ILoggerFactory *loggerFactory, 
					unsigned int width, 
					unsigned int height,
					std::unique_ptr<unsigned char> data){
			assert(loggerFactory != nullptr);
			assert(data != nullptr);
			this->width = width;
			this->height = height;
			this->data = std::move(data);
			this->sceneId = sceneId;
		}
		virtual IRenderable *getRenderable() = 0;
		std::string getSceneId(){
			return sceneId;
		}
		virtual void clearTextureData(){
			this->data.reset();
		}
		void setUnpackAlignment(CL_GLuint value){
			this->unpackAlignmentValue = value;
		}
	protected:
		std::string sceneId;
		std::unique_ptr<ILogger> logger = nullptr;
		unsigned int width;
		unsigned int height;
		std::unique_ptr<unsigned char> data = nullptr;
		CL_GLuint unpackAlignmentValue = 4;
	};
}

#endif //COREENGINE_TEXTURETEXT_H