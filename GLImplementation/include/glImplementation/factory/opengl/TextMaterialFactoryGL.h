#ifndef GLIMPLEMENTATION_TEXTMATERIALFACTORYGL_H
#define GLIMPLEMENTATION_TEXTMATERIALFACTORYGL_H

#include <coreEngine/ui/ITextMaterialFactory.h>
#include <glImplementation/ui/opengl/TextureTextGL.h>
#include <glImplementation/ui/opengl/MaterialTextGL.h>
#include <glImplementation/ui/opengl/ShaderTextGL.h>

namespace cl
{
class TextMaterialFactoryGL : public ITextMaterialFactory
{
  public:
    TextMaterialFactoryGL(ILoggerFactory *loggerFactory)
    {
	this->loggerFactory = loggerFactory;
    }
    std::unique_ptr<TextureText> createTexture(const std::string &sceneId,
					       const unsigned int &width,
					       const unsigned int &height,
					       std::unique_ptr<unsigned char> data)
    {

	return std::unique_ptr<TextureText>(new TextureTextGL(sceneId, loggerFactory, width, height, std::move(data)));
    }
    ShaderText *getShader(Scene *scene)
    {
	std::string shaderId = "ShaderTextGL";
	ShaderText *shaderText = nullptr;
	if (!(shaderText = (ShaderText *)scene->getFromScene(shaderId)))
	{
	    std::unique_ptr<ShaderText> shaderTextUptr(new ShaderTextGL(shaderId, loggerFactory, scene));
	    assert(shaderTextUptr != nullptr);
	    shaderText = shaderTextUptr.get();
	    scene->addToScene(std::move(shaderTextUptr));
	}
	return shaderText;
    }
    std::unique_ptr<MaterialText> createMaterial(const std::string &sceneId, ShaderText *shaderText, TextureText *textureText)
    {
	return std::unique_ptr<MaterialText>(new MaterialTextGL(sceneId, (ShaderTextGL *)shaderText, (TextureTextGL *)textureText, loggerFactory));
    }

  private:
    ILoggerFactory *loggerFactory = nullptr;
};
}

#endif //GLIMPLEMENTATION_TEXTMATERIALFACTORYGL_H