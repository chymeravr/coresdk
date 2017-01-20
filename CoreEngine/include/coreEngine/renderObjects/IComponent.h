#ifndef COREENGINE_ICOMPONENT_H
#define COREENGINE_ICOMPONENT_H

#include <string>
#include <coreEngine/renderObjects/IRenderable.h>
#include <coreEngine/renderObjects/Relation.h>

namespace cl{
	class IComponent{
	public:
		virtual std::string getComponentType() = 0;
		virtual IRenderable *getRenderable() = 0;
	};
}

#endif //COREENGINE_ICOMPONENT_H