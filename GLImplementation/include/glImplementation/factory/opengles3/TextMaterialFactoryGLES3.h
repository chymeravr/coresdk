#ifndef GLIMPLEMENTATION_TEXTMATERIALFACTORYGLES3_H
#define GLIMPLEMENTATION_TEXTMATERIALFACTORYGLES3_H

#include <coreEngine/ui/ITextMaterialFactory.h>
#include <glImplementation/ui/TextureTextGL.h>
#include <glImplementation/ui/opengles3/MaterialTextGLES3.h>
#include <glImplementation/ui/opengles3/ShaderTextGLES3.h>

namespace cl{
	class TextMaterialFactoryGLES3 : public ITextMaterialFactory{
	public:
		TextMaterialFactoryGLES3(ILoggerFactory *loggerFactory){
			this->loggerFactory = loggerFactory;
		}
		std::unique_ptr<TextureText> createTexture(const std::string &sceneId,
			const unsigned int &width,
			const unsigned int &height,
			std::unique_ptr<unsigned char> data){

			return std::unique_ptr<TextureText>(new TextureTextGL(sceneId, loggerFactory, width, height, std::move(data)));
		}
		ShaderText* getShader(Scene* scene){
			std::string shaderId = "ShaderTextGLES3";
			ShaderText *shaderText = nullptr;
			if (!(shaderText = (ShaderText*)scene->getFromScene(shaderId))){
				std::unique_ptr<ShaderText> shaderTextUptr(new ShaderTextGLES3(shaderId, loggerFactory, scene));
				assert(shaderTextUptr != nullptr);
				shaderText = shaderTextUptr.get();
				scene->addToScene(std::move(shaderTextUptr));
			}
			return shaderText;
		}
		std::unique_ptr<MaterialText> createMaterial(const std::string &sceneId, ShaderText *shaderText, TextureText *textureText){
			return std::unique_ptr<MaterialText>(new MaterialTextGLES3(sceneId, (ShaderTextGLES3*)shaderText, (TextureTextGL*)textureText, loggerFactory));
		}

	private:
		ILoggerFactory *loggerFactory = nullptr;
	};
}

#endif //GLIMPLEMENTATION_TEXTMATERIALFACTORYGLES3_H