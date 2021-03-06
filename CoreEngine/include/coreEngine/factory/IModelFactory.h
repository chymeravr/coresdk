#ifndef COREENGINE_IMODELFACTORY_H
#define COREENGINE_IMODELFACTORY_H

#include <memory>
#include <coreEngine/renderObjects/Model.h>

namespace cl{
	class IModelFactory{
	public:
		virtual std::unique_ptr<Model> create(const std::string &sceneId) = 0;
	};
}

#endif //COREENGINE_IMODELFACTORY_H