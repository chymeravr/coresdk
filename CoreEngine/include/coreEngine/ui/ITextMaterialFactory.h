#ifndef COREENGINE_ITEXTMATERIALFACTORY_H
#define COREENGINE_ITEXTMATERIALFACTORY_H

#include <memory>
#include <coreEngine/ui/TextureText.h>
#include <coreEngine/ui/MaterialText.h>
#include <coreEngine/ui/ShaderText.h>

namespace cl{
	class ITextMaterialFactory {
	public:
		virtual std::unique_ptr<TextureText> createTexture(const std::string &sceneId,
														const unsigned int &width,
														const unsigned int &height,
														std::unique_ptr<unsigned char> data) = 0;
		virtual std::unique_ptr<ShaderText> createShader(const std::string &sceneId, Scene* scene) = 0;
		virtual std::unique_ptr<MaterialText> createMaterial(const std::string &sceneId, ShaderText *shaderText, TextureText *textureText) = 0;
	};
}

#endif //COREENGINE_ITEXTMATERIALFACTORY_H
