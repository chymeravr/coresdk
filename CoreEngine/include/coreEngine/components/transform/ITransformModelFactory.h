#ifndef COREENGINE_ITRANSFORMMODELFACTORY_H
#define COREENGINE_ITRANSFORMMODELFACTORY_H

#include <memory>
#include <coreEngine/components/transform/TransformModel.h>

namespace cl{
	class ITransformModelFactory{
	public:
		virtual std::unique_ptr<TransformModel> create(Model *model) = 0;
	};
}

#endif //COREENGINE_ITRANSFORMMODELFACTORY_H