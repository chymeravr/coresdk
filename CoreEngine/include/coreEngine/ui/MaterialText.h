#ifndef COREENGINE_MATERIALTEXT_H
#define COREENGINE_MATERIALTEXT_H

#include <coreEngine/renderObjects/Material.h>
#include <coreEngine/ui/ShaderText.h>
#include <coreEngine/util/ILoggerFactory.h>
#include <coreEngine/ui/TextureText.h>

namespace cl{
	class MaterialText : public Material{
	public:
		virtual ~MaterialText(){}
		MaterialText(const std::string &sceneId, ShaderText *shaderText, TextureText *textureText, ILoggerFactory *loggerFactory):Material(sceneId, shaderText, loggerFactory){
			assert(textureText != nullptr);
			this->textureText = textureText;
		}
		void setTextColor(CL_Vec4 &color){
			this->color = color;
		}
		CL_Vec4 &getTextColor(){
			return this->color;
		}
		void setTextureText(TextureText *textureText){
			assert(textureText != nullptr);
			this->textureText = textureText;
		}
		TextureText *getTextureText(){
			return this->textureText;
		}
	protected:
		CL_Vec4 color = CL_Vec4(1.0, 1.0, 1.0, 1.0);
		TextureText *textureText = nullptr;
	};
}

#endif //COREENGINE_MATERIALTEXT_H