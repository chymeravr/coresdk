
#ifndef ANDROIDSDK_IMODELFACTORY_H
#define ANDROIDSDK_IMODELFACTORY_H

#include <memory>
#include <coreEngine/model/Object.h>
#include <coreEngine/model/Model.h>
#include <coreEngine/model/IModelRenderer.h>

namespace cl{
	class IModelFactory{
	public:
        virtual std::unique_ptr<Object> create(std::string tag, std::unique_ptr<IModelRenderer> modelRendererPtr) = 0;
        virtual void destroy(Model *modelPtr) = 0;
	};
}

#endif