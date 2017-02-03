#ifndef COREENGINE_ITEXTMATERIALFACTORY_H
#define COREENGINE_ITEXTMATERIALFACTORY_H

#include <memory>
#include <coreEngine/renderObjects/Texture.h>

namespace cl{
	class ITextMaterialFactory {
	public:
		virtual std::unique_ptr<Texture> createTexture(const std::string &sceneId) = 0;
	};
}

#endif //COREENGINE_ITEXTMATERIALFACTORY_H
