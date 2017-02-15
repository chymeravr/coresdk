#ifndef GLIMPLEMENTATION_TEXTMATERIALFACTORYGL_H
#define GLIMPLEMENTATION_TEXTMATERIALFACTORYGL_H

#include <coreEngine/ui/ITextMaterialFactory.h>
#include <glImplementation/ui/TextureTextGL.h>
#include <glImplementation/ui/MaterialTextGL.h>
#include <glImplementation/ui/ShaderTextGL.h>

namespace cl{
	class TextMaterialFactoryGL : public ITextMaterialFactory{
	public:
		TextMaterialFactoryGL(ILoggerFactory *loggerFactory){
			this->loggerFactory = loggerFactory;
		}
		std::unique_ptr<TextureText> createTexture(const std::string &sceneId,
			const unsigned int &width,
			const unsigned int &height,
			std::unique_ptr<unsigned char> data){

			return std::unique_ptr<TextureText>(new TextureTextGL(sceneId, loggerFactory, width, height, std::move(data)));
		}
		std::unique_ptr<ShaderText> createShader(const std::string &sceneId, Scene* scene){
			return std::unique_ptr<ShaderText>(new ShaderTextGL(sceneId, loggerFactory, scene));
		}
		std::unique_ptr<MaterialText> createMaterial(const std::string &sceneId, ShaderText *shaderText, TextureText *textureText){
			return std::unique_ptr<MaterialText>(new MaterialTextGL(sceneId, (ShaderTextGL*)shaderText, (TextureTextGL*)textureText, loggerFactory));
		}

	private:
		ILoggerFactory *loggerFactory = nullptr;
	};
}

#endif //GLIMPLEMENTATION_TEXTMATERIALFACTORYGL_H