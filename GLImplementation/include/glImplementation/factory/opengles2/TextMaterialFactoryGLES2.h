#ifndef GLIMPLEMENTATION_TEXTMATERIALFACTORYGLES2_H
#define GLIMPLEMENTATION_TEXTMATERIALFACTORYGLES2_H

#include <coreEngine/ui/ITextMaterialFactory.h>
#include <glImplementation/ui/opengles2/TextureTextGLES2.h>
#include <glImplementation/ui/opengles2/MaterialTextGLES2.h>
#include <glImplementation/ui/opengles2/ShaderTextGLES2.h>

namespace cl
{
class TextMaterialFactoryGLES2 : public ITextMaterialFactory
{
  public:
    TextMaterialFactoryGLES2(ILoggerFactory *loggerFactory)
    {
	this->loggerFactory = loggerFactory;
    }
    std::unique_ptr<TextureText> createTexture(const std::string &sceneId,
					       const unsigned int &width,
					       const unsigned int &height,
					       std::unique_ptr<unsigned char> data)
    {

	return std::unique_ptr<TextureText>(new TextureTextGLES2(sceneId, loggerFactory, width, height, std::move(data)));
    }
    ShaderText *getShader(Scene *scene)
    {
	std::string shaderId = "ShaderTextGLES2";
	ShaderText *shaderText = nullptr;
	if (!(shaderText = (ShaderText *)scene->getFromScene(shaderId)))
	{
	    std::unique_ptr<ShaderText> shaderTextUptr(new ShaderTextGLES2(shaderId, loggerFactory, scene));
	    assert(shaderTextUptr != nullptr);
	    shaderText = shaderTextUptr.get();
	    scene->addToScene(std::move(shaderTextUptr));
	}
	return shaderText;
    }
    std::unique_ptr<MaterialText> createMaterial(const std::string &sceneId, ShaderText *shaderText, TextureText *textureText)
    {
	return std::unique_ptr<MaterialText>(new MaterialTextGLES2(sceneId, (ShaderTextGLES2 *)shaderText, (TextureTextGLES2 *)textureText, loggerFactory));
    }

  private:
    ILoggerFactory *loggerFactory = nullptr;
};
}

#endif //GLIMPLEMENTATION_TEXTMATERIALFACTORYGLES2_H